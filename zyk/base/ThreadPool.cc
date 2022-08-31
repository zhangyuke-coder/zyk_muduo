#include "ThreadPool.h"
#include <iostream>
#include <assert.h>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
using namespace zyk;

ThreadPool::ThreadPool(const std::string& nameArg) 
    : mutex_(),
      notEmpty_(),
      notFull_(),
      name_(nameArg),
      maxQueueSize_(0),
      running_(false){
}
ThreadPool::~ThreadPool() {
    if(running_){
        stop();
    }
}

void ThreadPool::start(int numThreads) {
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    for(int i = 0; i < numThreads; i++) {
        char id[32];
        snprintf(id, 32, "%d", i + 1);
        std::unique_ptr<std::thread> t(new std::thread(std::bind(&ThreadPool::runInThread, this)));
 
        pthread_setname_np(t->native_handle(), id);
        threads_.emplace_back(std::move(t));
        // threads_.emplace_back(new std::thread(std::bind(&ThreadPool::runInThread, this)));

    }
    if(numThreads == 0 && threadInitCallback_) {
        threadInitCallback_();
    }
}

void ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lk(mutex_);
    
        running_ = false;
        notEmpty_.notify_all();
        notFull_.notify_all();

    }


    for(auto& thr : threads_) {
        thr->join();
    }
    std::cout << "stop " << std::endl;
}

size_t ThreadPool::queueSize() const {
    std::lock_guard<std::mutex> lk(mutex_);
    return maxQueueSize_;
}

void ThreadPool::run(const Task& f) {
    if(threads_.empty()) {
        f();
    } else {
        std::unique_lock<std::mutex> lk(mutex_);
        notFull_.wait(lk, [this] { return !isFull() || !running_; });
        if(!running_) return;
        assert(!isFull());
        queue_.emplace_back(f);
        notEmpty_.notify_one();
    }
}

ThreadPool::Task ThreadPool::take() {
    std::unique_lock<std::mutex> lk(mutex_);
    notEmpty_.wait(lk, [this] { return !queue_.empty() || !running_; });
    ThreadPool::Task task;
    if(!queue_.empty()) {
        task = queue_.front();
        queue_.pop_front();
        if(maxQueueSize_ > 0) {
            notFull_.notify_one();
        }
    }
    return task;
}

bool ThreadPool::isFull() const {
    return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

void ThreadPool::runInThread() {
    
    try {
        if(threadInitCallback_) {
            threadInitCallback_();
        }

        while(running_) {
            Task task = take();
            if(task) {
                task();
            }
        }
        
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        fprintf(stderr, "stack trace: %s\n", boost::stacktrace::stacktrace());
        abort();
    }
    catch(...) {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
        throw; // rethrow
    }
}
