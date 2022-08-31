#include "FileUtil.h"
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>  // for off_t
#include <stdio.h>
#include "Types.h"
using namespace zyk;

FileUtil::AppendFile::AppendFile(StringArg filename) 
    : fp_(::fopen(filename.c_str(), "ae")),
      writtenBytes_(0)
{
    // assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::AppendFile::~AppendFile() {
    ::fclose(fp_);
}
void FileUtil::AppendFile::append(const char* logline, const size_t len) {
    size_t written = 0;
    while(written != len) {
        size_t remain = len - written;
        size_t n = write(logline + written, remain);
        if(n != remain) {
            int err = ferror(fp_);
            if(err) {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
                break;
            }

        }
        written += n;
    }
    writtenBytes_ += written;
}
void FileUtil::AppendFile::flush()
{
  ::fflush(fp_);
}

size_t FileUtil::AppendFile::write(const char* logline, size_t len)
{
  // #undef fwrite_unlocked
  return ::fwrite_unlocked(logline, 1, len, fp_);
}

FileUtil::ReadSmallFile::ReadSmallFile(StringArg filename)
  : fd_(::open(filename.c_str(), O_RDONLY | O_CLOEXEC)),
    err_(0)
{
  buf_[0] = '\0';
  if (fd_ < 0)
  {
    err_ = errno;
  }
}

FileUtil::ReadSmallFile::~ReadSmallFile()
{
  if (fd_ >= 0)
  {
    ::close(fd_); // FIXME: check EINTR
  }
}

template <typename String>

int FileUtil::ReadSmallFile::readToString(int maxsize,
										 String* content,
										 int64_t* fileSize,
										 time_t* modifyTime,
										 time_t* createTime)
{
	static_assert(sizeof(off_t) == 8, "_FILE_OFFSET_BITS = 64");
	assert(content != NULL);
	int err = err_;
	if(fileSize) {
		struct stat statbuf;
		if(::fstat(fd_, &statbuf)) {
			if(S_ISREG(statbuf.st_mode)) {
				*fileSize = statbuf.st_size;
				content->reserve(static_cast<int>(std::min(implicit_cast<int64_t>(maxsize), *fileSize)));

			}
			else if(S_ISDIR(statbuf.st_mode)) {
				err = EISDIR;
			}
			if(modifyTime) {
				*modifyTime = statbuf.st_mtime;
			}
			if(createTime) {
				*createTime = statbuf.st_ctime;
			}
		} else {
			err = errno;
		}
	}


	while(content->size() < implicit_cast<size_t>(maxsize)) {
		size_t toRead = std::min(implicit_cast<size_t>(maxsize) - content->size(), sizeof(buf_));
		ssize_t n = ::read(fd_, buf_, toRead);
		if(n > 0) {
			content->append(buf_, n);
		}
		else {
			if( n < 0) {
				err = errno;
			}
			break;
		}
	}
	return err;
}
int FileUtil::ReadSmallFile::readToBuffer(int* size)
{
  int err = err_;
  if (fd_ >= 0)
  {
    ssize_t n = ::pread(fd_, buf_, sizeof(buf_)-1, 0);
    if (n >= 0)
    {
      if (size)
      {
        *size = static_cast<int>(n);
      }
      buf_[n] = '\0';
    }
    else
    {
      err = errno;
    }
  }
  return err;
}

template int FileUtil::readFile(StringArg filename,
                                int maxSize,
                                string* content,
                                int64_t*, int64_t*, int64_t*);

template int FileUtil::ReadSmallFile::readToString(
    int maxSize,
    string* content,
    int64_t*, int64_t*, int64_t*);
