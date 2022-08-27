cmake_minimum_required(VERSION 2.8)
include_directories(../)
add_executable(main
               ../Timestamp.cc
               ../CountDownLatch.cc
               ../ThreadPool.cc
               testTimestamp.cc)
target_link_libraries(main -lpthread)