#include <iostream>
#include "../Logging.h"
#include "../ThreadPool.h"
using namespace std;
using namespace zyk;
void print(){
    LOG_INFO << "zyk";
}
int main() {
    ThreadPool p("zyk");
    p.start(100);
    for(int i = 0; i < 100; i++) {
        p.run(std::bind(print));
    }

}