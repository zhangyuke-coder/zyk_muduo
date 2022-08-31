#include "../LogStream.h"
#include <iostream>
using namespace std;
using namespace zyk;
int main() {
    LogStream ss;
    ss << "asdf";
    cout << ss.buffer().toString();
}