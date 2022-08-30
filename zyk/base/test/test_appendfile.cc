#include "../FileUtil.h"
#include <iostream>
#include <time.h>
#include <thread>
using namespace zyk;
int main() {
    FileUtil::AppendFile f("./test.txt"); 
    f.append("asdf\n", 5);
    f.flush();
    std::this_thread::sleep_for(std::chrono::minutes(1));

}