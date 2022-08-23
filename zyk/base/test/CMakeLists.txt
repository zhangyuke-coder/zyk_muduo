cmake_minimum_required(VERSION 2.8)
include_directories(../)
add_executable(main 
               ../CountDownLatch.cc
               ../ThreadPool.cc
               test_ThreadPool.cc)
target_link_libraries(main -lpthread)