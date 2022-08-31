#include "../ThreadPool.h"
#include "../CountDownLatch.h"
#include <iostream>

#include <thread>
using namespace std;
using namespace zyk;
void print() {
    printf("tid = %d\n", std::this_thread::get_id());

}

void printString(const std::string& str) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "zyk" << str <<std::endl;
}

void test(int maxSize) {
    ThreadPool pool("zyk_thread_pool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);
    cout << "Adding" << endl;
    for(int i = 0; i < 100; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "task %d", i);
        pool.run(std::move(std::bind(printString, std::string(buf))));
    }
    cout << "Done" << endl;
    CountDownLatch latch(1);
    pool.run(std::move(std::bind(&zyk::CountDownLatch::countDown, &latch)));
    latch.wait();

    pool.stop();
}

void longTask(int num) {
    cout << "longTask" << num << endl;
    this_thread::sleep_for(std::chrono::seconds(10));
    cout << "faile" << endl;
}


void test2() {
    ThreadPool pool("zyt_test2");
    pool.setMaxQueueSize(5);
    pool.start(3);
    thread t ([&pool](){
        for(int i = 0; i < 20; ++i) {
            pool.run(std::bind(longTask, i));
        }
    });
    cout << "test2_first" << endl;
    this_thread::sleep_for(chrono::seconds(3));
    pool.stop();
    t.join();

}
void f(int i) {
    cout << i << endl;
}
void test3() {
    ThreadPool pool("zyk");
    pool.setMaxQueueSize(2);
    pool.start(3);
    for(int i = 0; i < 10; ++i) {
        pool.run(std::bind(f, i));
    }
    pool.stop();
    
}
int main() {
    // test(0);
    // test(1);
    // test(5);
    // test(10);
    // test(50);
    test(3);
}