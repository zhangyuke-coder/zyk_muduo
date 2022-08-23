#pragma once 

#include <mutex>
#include <condition_variable>
#include "noncopyable.h"

namespace zyk {

class CountDownLatch : zyk::noncopyable{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount() const;






public:
    int count_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;

};



}
