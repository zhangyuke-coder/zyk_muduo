#include <iostream>

#include <time.h>

using namespace std;
int main(){
    // char timebuf[32];
    // struct tm tm;
    // time_t now = time(NULL);
    // gmtime_r(&now, &tm); // FIXME: localtime_r ?
    // // strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
    // snprintf(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    // cout << timebuf;
    time_t now;
    now = time(NULL);
    cout << now << endl;
    int x = 2;
    cout << (float)now / x;
}




