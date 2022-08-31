#include "../FileUtil.h"
#include <iostream>
#include <string>
using namespace std;
using namespace zyk;

int main() {
    std::string content;
    int64_t fileSize;
    time_t modifyTime;
    time_t createTime;
    FileUtil::readFile("test.txt", 1, &content, &fileSize, &modifyTime, &createTime); 
    cout << content;
}