#include "LogFile.h"
#include "FileUtil.h"
#include "ProcessInfo.h"
#include <assert.h>
#include <iostream>
using namespace zyk;

logFile::logFile(const std::string& basename,
            off_t rollSize,
            int flushInterval,
            int checkEveryN)
            : basename_(basename),
              rollSize_(rollSize),
              count_(0),
              flushInterval_(flushInterval),
              checkEveryN_(checkEveryN),
              startOfPeriod_(0),
              lastRoll_(0),
              lastFlush_(0)
{
    assert(basename.find('/') == std::string::npos);
    rollFile();
}

logFile::~logFile() = default;

std::string logFile::getLogFileName(const std::string& basename, time_t* now) {
    char timebuf[32];
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;
    filename += ProcessInfo::hostname();

    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), ".%d", ProcessInfo::pid());
    filename += pidbuf;

    filename += ".log";
    return filename;

}
bool logFile::rollFile() {
    time_t now;
    std::string filename = getLogFileName(basename_, &now);

    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    if(now > lastRoll_) {
        lastRoll_ = now;
        lastFlush_ = now;
        startOfPeriod_ = start;
        file_.reset(new FileUtil::AppendFile(filename));
        return true;
    }
    return false;
}
void logFile::append_unlocked(const char* logline, int len) {
    file_->append(logline, len);
    if(file_->writtenBytes() > rollSize_) {
        rollFile();
    } else {
        ++count_;
        if(count_ >= checkEveryN_) {
            count_ = 0;
            time_t now = ::time(NULL);
            time_t thisPeriod = now /kRollPerSeconds_ * kRollPerSeconds_;
            if(thisPeriod != startOfPeriod_) {
                rollFile();
            }
            else if(now - lastFlush_ > flushInterval_) {
                lastFlush_ = now;
                file_->flush();
            }
        }
    }
}
void logFile::flush() {
    std::lock_guard<std::mutex> lk(mutex_);
    file_->flush(); 
}

void logFile::append(const char* logline, int len) {
    std::lock_guard<std::mutex> lk(mutex_);
    append_unlocked(logline, len);
}