LIBS=../build/libleveldb.a
CPPFLAGS=-I ../build/include -I ../include -I .. -std=c++11 -g -DLEVELDB_PLATFORM_POSIX -DLEVELDB_HAS_PORT_CONFIG_H
LDFLAGS=-lpthread

sample:sample.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

test:test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

version_edit_test:version_edit_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

version_set_test:version_set_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

posix_writable_file_test:posix_writable_file_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

log_writer_test:log_writer_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

log_reader_test:log_reader_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

log_writer_blob_test:log_writer_blob_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

filename_test:filename_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

env_test:env_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

log_test:log_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

crc32c_test:crc32c_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

dbformat_test:dbformat_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

table_builder_test:table_builder_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

comparator_test:comparator_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

block_builder_test:block_builder_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

snapshot_test:snapshot_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

bloom_test:bloom_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

filter_block_test:filter_block_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

table_test:table_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

cache_test:cache_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)

manifest_reader_test:manifest_reader_test.cpp
	$(CXX) -o $@ $< $(LIBS) $(CPPFLAGS) $(LDFLAGS)
