#include "CountDownLatch.h"
#include <iostream>
using namespace zyk;

CountDownLatch::CountDownLatch(int count)
                    : count_(count),
                      mutex_(),
                      cv_() {

                      }




void CountDownLatch::wait() {
    std::unique_lock<std::mutex> lk(mutex_);


    cv_.wait(lk, [this] {return count_ <= 0;} );
}


void CountDownLatch::countDown() {
    std::lock_guard<std::mutex> lk(mutex_);
    --count_;
    if(count_ == 0) {
        cv_.notify_all();
    }
}

int CountDownLatch::getCount() const {
    std::lock_guard<std::mutex> lk(mutex_);
    return count_;

}
