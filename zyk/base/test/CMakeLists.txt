cmake_minimum_required(VERSION 2.8)
include_directories(../)
add_executable(main
               ../Timestamp.cc
               ../CountDownLatch.cc
               ../ThreadPool.cc
               ../FileUtil.cc
               ../LogFile.cc
               ../ProcessInfo.cc
               ../Logging.cc
               ../LogStream.cc
               ../AsyncLogging.cc
               test_logging.cc)
target_link_libraries(main -lpthread)