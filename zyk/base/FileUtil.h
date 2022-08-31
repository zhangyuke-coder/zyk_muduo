#pragma once
#include "noncopyable.h"
#include "StringPiece.h"
namespace zyk {
namespace FileUtil {
class ReadSmallFile : noncopyable {

public:
    ReadSmallFile(StringArg filename);
    ~ReadSmallFile();
    template <typename String>
    int readToString(int maxsize,
                     String* constent,
                     int64_t* fileSize,
                     time_t* modifyTime,
                     time_t* createTime);
    static const int kBufferSize = 64 * 1024;

    int readToBuffer(int* size);
    const char* buffer() const {
        return buf_;
    }



private:
    int fd_;
    int err_;
    char buf_[kBufferSize];




};



template<typename String>
int readFile(StringArg filename,
             int maxSize,
             String* content,
             int64_t* fileSize = NULL,
             int64_t* modifyTime = NULL,
             int64_t* createTime = NULL)
{
  ReadSmallFile file(filename);
  return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

class AppendFile : noncopyable
{
 public:
  explicit AppendFile(StringArg filename);

  ~AppendFile();

  void append(const char* logline, size_t len);

  void flush();

  off_t writtenBytes() const { return writtenBytes_; }

 private:

  size_t write(const char* logline, size_t len);

  FILE* fp_;
  char buffer_[64*1024];
  off_t writtenBytes_;
};




}
}