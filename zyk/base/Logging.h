
#include "LogStream.h"
#include "Timestamp.h"
#include <functional>

namespace zyk {

class Logger {


public:
    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    class SourceFile {


    public:
        template <int N>
        SourceFile(const char (&arr)[N]) 
            : data_(arr),
              size_(N - 1)
        {
            const char* slash = strrchr(data_, '/');
            if(slash) {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }
        explicit SourceFile(const char* filename) 
            : data_(filename) {
            const char* slash = strrchr(filename, '/');
            if(slash) {
                data_ = slash + 1;

            }

            size_ -= static_cast<int>(strlen(data_));
        }


        const char* data_;
        int size_;
    };


    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);

    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();
    LogStream& stream() { return impl_.stream_; }
    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    using OutputFunc = std::function<void(const char* msg, int len)>;
    using FlushFunc = std::function<void()>;
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);

    private:
        class Impl {
            public:

                Impl(LogLevel level, const SourceFile& file, int line);
                void finish();
                Timestamp time_;
                LogStream stream_;
                LogLevel level_;
                int line_;
                SourceFile basename_;
        };
        Impl impl_;



};


extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{

  return g_logLevel;
}


#define LOG_TRACE if (zyk::Logger::logLevel() <= zyk::Logger::LogLevel::TRACE) \
  zyk::Logger(__FILE__, __LINE__, zyk::Logger::LogLevel::TRACE, __func__).stream()
#define LOG_DEBUG if (zyk::Logger::logLevel() <= zyk::Logger::LogLevel::DEBUG) \
  zyk::Logger(__FILE__, __LINE__, zyk::Logger::LogLevel::DEBUG, __func__).stream()
#define LOG_INFO if (zyk::Logger::logLevel() <= zyk::Logger::LogLevel::INFO) \
  zyk::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN zyk::Logger(__FILE__, __LINE__, zyk::Logger::LogLevel::WARN).stream()
#define LOG_ERROR zyk::Logger(__FILE__, __LINE__, zyk::Logger::LogLevel::ERROR).stream()
#define LOG_FATAL zyk::Logger(__FILE__, __LINE__, zyk::Logger::LogLevel::FATAL).stream()
#define LOG_SYSERR zyk::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL zyk::Logger(__FILE__, __LINE__, true).stream()


}