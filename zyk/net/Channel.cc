#include "Channel.h"
#include <sstream>
#include <poll.h>
using namespace zyk;

using namespace net;
Channel::Channel(EventLoop* loop,
                int fd) :
    loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    logHup_(true),
    eventHandling_(false),
    addedToLoop_(false)
{}

Channel::~Channel() {
    assert(!eventHandling_);
    assert(!addedToLoop_);

}




string Channel::reventsToString() const
{
    return eventsToString(fd_, revents_);
}

string Channel::eventsToString() const
{
    return eventsToString(fd_, events_);
}
string Channel::eventsToString(int fd, int ev)
{
  std::ostringstream oss;
  oss << fd << ": ";
  if (ev & POLLIN)
    oss << "IN ";
  if (ev & POLLPRI)
    oss << "PRI ";
  if (ev & POLLOUT)
    oss << "OUT ";
  if (ev & POLLHUP)
    oss << "HUP ";
  if (ev & POLLRDHUP)
    oss << "RDHUP ";
  if (ev & POLLERR)
    oss << "ERR ";
  if (ev & POLLNVAL)
    oss << "NVAL ";

  return oss.str();
}
void update() {

}