#include <iostream>
#include <thread>
#include "../ThreadPool.h"
#include "../LogFile.h"
using namespace std;
using namespace zyk;
logFile f("heihei", 1024);
void print(int i) {
    // char buf[32];
    // snprintf(buf, sizeof(buf), "%d", i);
    // f.append(buf, 32);
    f.append("asdf\n", 5);
}
int main() {
    ThreadPool p("zyk");
    p.start(10);
    for(int i = 0; i < 3; i++) {
        p.run(std::bind(print, i));
    }
    p.stop();
}