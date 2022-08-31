#include "EpollPoller.h"
#include "../base/Logging.h"
#include "../Channel.h"
#include <poll.h>
#include "../base/Timestamp.h"
#include <sys/epoll.h>
#include "../base/Types.h"
using namespace zyk;
using namespace zyk::net;

namespace {
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}

EPollPoller::EPollPoller(EventLoop* loop)
    : Poller(loop),
      epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(kInitEventListSize) {
        if(epollfd_ < 0) {
            LOG_SYSFATAL << "EPollPoller::EPollPoller";
        }  
      }
EPollPoller::~EPollPoller() {
    ::close(epollfd_);
}

Timestamp EPollPoller::poll(int timeoutMs, ChannelList* activeChannels) {
    LOG_TRACE << "fd total count " << channels_.size();
    int numEvents = ::epoll_wait(epollfd_, 
                                 &*events_.begin(),
                                 static_cast<int>(events_.size()),
                                 timeoutMs);
                                
    int savedErrno = errno;
    Timestamp now(Timestamp::now());                            
    
    if(numEvents > 0) {
        LOG_TRACE << numEvents << " events happened";
        fillActiveChannels(numEvents, activeChannels);
        if(implicit_cast<size_t>(numEvents) == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    } else if(numEvents == 0) {
        LOG_TRACE << "nothing happened";
    } else {
        if(savedErrno != EINTR) {
            errno = savedErrno;
            LOG_SYSERR << "EPollPoller::poll()";
        }
    }
    return now;
}

void EPollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const {
    assert(implicit_cast<size_t>(numEvents) <= events_.size());
    for(int i = 0; i < numEvents; ++i) {

    }
}

void EPollPoller::updateChannel(Channel* channel) {
    const int index = channel->index();
    LOG_TRACE << "fd = " << channel->fd()
    << " events = " << channel->events() << " index = " << index;
    if (index == kNew || index == kDeleted)
    {
    // a new one, add with EPOLL_CTL_ADD
        int fd = channel->fd();
        if (index == kNew)
        {
            assert(channels_.find(fd) == channels_.end());
            channels_[fd] = channel;
        }
        else // index == kDeleted
        {
            assert(channels_.find(fd) != channels_.end());
            assert(channels_[fd] == channel);
        }

        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
}