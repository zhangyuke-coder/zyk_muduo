#include "../AsyncLogging.h"
#include "../Logging.h"
#include "../Timestamp.h"

#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <iostream>
using namespace std;
using namespace zyk;

int main() {
    AsyncLogging log("zyk", 3);
    log.start();
    log.append("asdfafg", 7);
    sleep(4);
    cout << "stop" << endl;
    log.stop();
}