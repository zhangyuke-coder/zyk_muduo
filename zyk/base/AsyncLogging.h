#pragma once 
#include <thread>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <functional>
#include "LogStream.h"
#include "CountDownLatch.h"
#include "noncopyable.h"
namespace zyk {
class AsyncLogging : noncopyable {
public:
    AsyncLogging(const string& basename,
                 off_t rollSize,
                 int flushInterval = 3);
    ~AsyncLogging() {
        if(running_) {
            stop();
        }
    }
    void append(const char* logline, int len);

    void start() {
        running_ = true;
        thread_.reset(new std::thread(std::bind(&AsyncLogging::threadFunc, this)));
        latch_.wait();
    }

    void stop() {
        running_ = false;
        cond_.notify_one();
        thread_->join();
        
    }
private:
    void threadFunc();
    using Buffer = detail::FixedBuffer<detail::kLargeBuffer>;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;

    const int flushInterval_;
    std::atomic<bool> running_;
    const string basename_;
    const off_t rollSize_;
    std::mutex mutex_;
    std::unique_ptr<std::thread> thread_;
    CountDownLatch latch_;
    std::condition_variable cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;







};
};