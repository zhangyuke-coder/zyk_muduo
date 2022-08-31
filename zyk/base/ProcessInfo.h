#pragma once
#include <sys/types.h>
#include <string>
#include "Timestamp.h"
#include "StringPiece.h"
#include <vector>
namespace zyk {

namespace ProcessInfo {

pid_t pid();
std::string pidString();
uid_t uid();
std::string username();
uid_t euid();
Timestamp startTime();
int clockTicksPerSecond();
int pageSize();
bool isDebugBuild();  // constexpr

string hostname();
string procname();
StringPiece procname(const string& stat);

/// read /proc/self/status
string procStatus();

/// read /proc/self/stat
string procStat();

/// read /proc/self/task/tid/stat
string threadStat();

/// readlink /proc/self/exe
string exePath();

int openedFiles();
int maxOpenFiles();


struct CpuTime
{
    double userSeconds;
    double systemSeconds;

    CpuTime() : userSeconds(0.0), systemSeconds(0.0) { }

    double total() const { return userSeconds + systemSeconds; }
};
CpuTime cpuTime();

int numThreads();
std::vector<pid_t> threads();

}
}