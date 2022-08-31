#pragma once
#include <map>
#include <vector>
#include "../base/noncopyable.h"
#include "../base/Timestamp.h"
namespace zyk {

namespace net {
class Channel;
class EventLoop;
class Poller : noncopyable {

public:
    using ChannelList = std::vector<Channel*>;
    Poller(EventLoop* loop);
    virtual ~Poller();
    virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;
    virtual bool hasChannel(Channel* channel) const ;
    static Poller* newDefaultPoller(EventLoop* loop);
    
protected:
    using ChannelMap = std::map<int, Channel*>;
    ChannelMap channels_;
private:
    EventLoop* ownerLoop_;



};
}


}