// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/log_writer.h"

#include <stdint.h>
#include "leveldb/env.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {
namespace log {

static void InitTypeCrc(uint32_t* type_crc) {
  for (int i = 0; i <= kMaxRecordType; i++) {
    char t = static_cast<char>(i);
    type_crc[i] = crc32c::Value(&t, 1);
  }
}

Writer::Writer(WritableFile* dest)
    : dest_(dest),
      block_offset_(0) {
  InitTypeCrc(type_crc_);
}

Writer::Writer(WritableFile* dest, uint64_t dest_length)
    : dest_(dest), block_offset_(dest_length % kBlockSize) {
  InitTypeCrc(type_crc_);
}

Writer::~Writer() {
}

//slice数据写入底层文件，可能会分多次写入
Status Writer::AddRecord(const Slice& slice) {
  const char* ptr = slice.data();
  size_t left = slice.size();

  // Fragment the record if necessary and emit it.  Note that if slice
  // is empty, we still want to iterate once to emit a single
  // zero-length record
  Status s;
  bool begin = true;
  do {
    //leftover记录block当前可用大小
    const int leftover = kBlockSize - block_offset_;
    assert(leftover >= 0);
    //如果block可用大小已经无法写入header，那么补充\x00
    if (leftover < kHeaderSize) {
      // Switch to a new block
      if (leftover > 0) {
        // Fill the trailer (literal below relies on kHeaderSize being 7)
        assert(kHeaderSize == 7);
        //Slice构造函数第二个参数表示字符串长度，因此效果上是leftover个'\x00'
        //leftover size最大为6，因此第一个参数指定6个'\x00'
        dest_->Append(Slice("\x00\x00\x00\x00\x00\x00", leftover));
      }
      block_offset_ = 0;
    }

    // Invariant: we never leave < kHeaderSize bytes in a block.
    // block还有剩余可用空间让我们写入一部分数据
    // 如果 = 0，也就是剩余只能写入header，那也写入（只是此时不会写入slice里任何数据）
    assert(kBlockSize - block_offset_ - kHeaderSize >= 0);

    //可用大小为kBlockSize - block_offset_，减去header大小即为可写的数据大小
    const size_t avail = kBlockSize - block_offset_ - kHeaderSize;
    //能够全部写入则=left，否则等于可写大小
    const size_t fragment_length = (left < avail) ? left : avail;

    RecordType type;
    const bool end = (left == fragment_length);//相等表示本次可以全部写入
    if (begin && end) {
      type = kFullType;//数据能够一次写入
    } else if (begin) {
      type = kFirstType;//数据无法一次写入时，标记首次写入
    } else if (end) {
      type = kLastType;//数据无法一次写入时，标记最后一次写入
    } else {
      type = kMiddleType;//数据无法一次写入时，标记中间写入
    }

    s = EmitPhysicalRecord(type, ptr, fragment_length);
    ptr += fragment_length;//下一次slice待写入位置
    left -= fragment_length;//slice还有多少没有写
    begin = false;
  } while (s.ok() && left > 0);
  return s;
}

//写入header data到底层文件(调用flush确保写入)
//|crc(4B)  |length(2B)  |type(1B)  |ptr(nB)...  |
//|--------------header-------------|----data----|
Status Writer::EmitPhysicalRecord(RecordType t, const char* ptr, size_t n) {
  //assert这里对应n要使用buf[4, 5]存储，一共两个字节，因此大小应当<=0xffff
  assert(n <= 0xffff);  // Must fit in two bytes
  //要写入的数据大小一定能够写入(不大于kBlockSize)
  assert(block_offset_ + kHeaderSize + n <= kBlockSize);

  // Format the header
  char buf[kHeaderSize];
  //长度使用两个字节(buf[4, 5])存储，其中4存储低两位，5存储高两位
  //例如n=0x6789，则buf[4]=0x89，buf[5]=0x67
  buf[4] = static_cast<char>(n & 0xff);
  buf[5] = static_cast<char>(n >> 8);
  //buf[6]存储类型
  buf[6] = static_cast<char>(t);

  //计算crc: Extend(type_crc, ptr) -> Mask -> EncodeFixed32
  // Compute the crc of the record type and the payload.
  uint32_t crc = crc32c::Extend(type_crc_[t], ptr, n);
  crc = crc32c::Mask(crc);                 // Adjust for storage
  EncodeFixed32(buf, crc);

  // Write the header and the payload
  // 首先写入header: |crc    |length  |type |，大小为kHeaderSize
  Status s = dest_->Append(Slice(buf, kHeaderSize));
  if (s.ok()) {
    //接着写入数据，大小为n
    s = dest_->Append(Slice(ptr, n));
    if (s.ok()) {
      s = dest_->Flush();
    }
  }
  block_offset_ += kHeaderSize + n;
  return s;
}

}  // namespace log
}  // namespace leveldb
