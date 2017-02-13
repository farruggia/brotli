project(brotli CXX C)
set(PROJECT_VENDOR "Andrea Farruggia")
set(PROJECT_CONTACT "a.farruggia@di.unipi.it")
set(PROJECT_DESCRIPTION "Brotli compressor - CMake repackaging")

cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

option(BRO_TOOLS "Build tools")

# C++11 support, Release by default
include(Cpp14)
include(NinjaColorFix)
include(ReleaseDefault)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_FASTDEB "-g -Wall -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native -fno-omit-frame-pointer -no-canonical-prefixes -DNDEBUG")
set(CMAKE_CXX_FLAGS_FAST "-O3 -march=native -fno-omit-frame-pointer -no-canonical-prefixes -DNDEBUG -ffast-math -funroll-loops")

include_directories("${PROJECT_SOURCE_DIR}/include/brotli")

# Library
add_library(libbrotli 
  lib/dec/state.c 
  lib/dec/huffman.c 
  lib/dec/bit_reader.c 
  lib/dec/decode.c 
  lib/dictionary.cc 
  lib/enc/histogram.cc 
  lib/enc/encode.cc 
  lib/enc/compress_fragment_two_pass.cc 
  lib/enc/streams.cc 
  lib/enc/utf8_util.cc 
  lib/enc/brotli_bit_stream.cc 
  lib/enc/metablock.cc 
  lib/enc/backward_references.cc 
  lib/enc/block_splitter.cc 
  lib/enc/static_dict.cc 
  lib/enc/entropy_encode.cc 
  lib/enc/compress_fragment.cc 
  lib/enc/literal_cost.cc 
)

if (BRO_TOOLS)
  add_subdirectory(tools)
endif(BRO_TOOLS)
