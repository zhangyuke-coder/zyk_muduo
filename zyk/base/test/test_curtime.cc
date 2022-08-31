#include <cstdio>
#include <ctime>
 
int main(int argc, char* argv[]) {
    time_t rawtime;
    struct tm *ptminfo;
 
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    printf("current: %02d-%02d-%02d %02d:%02d:%02d\n",
            ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
            ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
    return 0;
}