#include <iostream>
#include "../CountDownLatch.h"
#include <thread>

using namespace zyk;
CountDownLatch latch(2);

void f1() {
    int n = 2;
    while(n--) {
        // std::cout << latch.count_ << std::endl;
        latch.countDown();
    }
   
}
void f2() {

   latch.wait();
   std::cout << latch.getCount() << std::endl;
}
int main() {
    // std::thread t1(f1);
    // std::thread t2(f2);
    
    // t1.join();
    // t2.join();
    char s[5] = "asdf";
    std::cout << s << std::endl;
}