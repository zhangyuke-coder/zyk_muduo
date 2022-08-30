#include "../LogFile.h"
#include "../ProcessInfo.h"
#include <iostream>
using namespace std;
int main() {
    zyk::logFile f("heihei", 1, 1, 1);

    f.rollFile();


}