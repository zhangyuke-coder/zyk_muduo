#pragma once 
#include "noncopyable.h"
#include <string>
#include <memory>
#include <mutex>
namespace zyk {
namespace FileUtil
{
class AppendFile;
} // namespace FileUtil




class logFile : noncopyable {
public:
    logFile(const std::string& basename,
            off_t rollSize,
            int flushInterval = 3,
            int checkEveryN = 1024);
    ~logFile();

    void append(const char* logline, int len);
    void flush();
    bool rollFile();


private:
    void append_unlocked(const char* logline, int len);
    static std::string getLogFileName(const std::string& basename, time_t* now);
    std::string basename_;
    const off_t rollSize_;
    const int checkEveryN_;
    const time_t flushInterval_;
    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<FileUtil::AppendFile> file_;
    int count_;
    const static int kRollPerSeconds_ = 60 * 60 * 24;
    std::mutex mutex_;
};
}