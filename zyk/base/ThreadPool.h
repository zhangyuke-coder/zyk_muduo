#pragma once 
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "noncopyable.h"
#include <deque>

#include <vector>
namespace zyk {

class ThreadPool : zyk::noncopyable {
public:
    using Task = std::function<void ()> ;
    explicit ThreadPool(const std::string& nameArg = std::string("ThreadPool"));

    ~ThreadPool();

    void setMaxQueueSize(size_t maxSize) { maxQueueSize_ = maxSize; }
    void setThreadInitCallback(Task&& cb) {
        threadInitCallback_ = cb;
    }

    void start(int numThreads);
    void stop();
    const std::string& name() const {
        return name_;
    }

    void run(const Task& f);

    size_t queueSize() const;
private:

    bool isFull() const;
    void runInThread();
    Task take();


    bool running_;
    mutable std::mutex mutex_;
    std::deque<Task> queue_;
    // std::vector<std::thread> threads_;
    std::vector<std::unique_ptr<std::thread>> threads_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::string name_;
    std::size_t maxQueueSize_;
    Task threadInitCallback_;
};

}