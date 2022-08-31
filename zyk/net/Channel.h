#pragma once


#include <functional>
#include "../base/Timestamp.h"
namespace zyk {
namespace net{

class EventLoop;
class Channel {
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent();
    void setReadCallback(ReadEventCallback cb){ 
        readCallback_ = std::move(cb); 
    }
    void setWriteCallback(EventCallback cb) {
        writeCallback_ = std::move(cb);
    }
    void setCloseCallback(EventCallback cb) {
        closeCallback_ = std::move(cb);
    }
    void setErrorCallback(EventCallback cb) {
        errorCallback_ = std::move(cb);
    }


    int fd() const {
        return fd_;
    }

    int events() const {
        return events_;
    }
    void set_revents(int revt) {
        revents_ = revt;
    }

    bool isNoneEvents() const {
        return events_ == kNoneEvent;
    }
    void enableReading() { events_ |= kReadEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoneEvent; update(); }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }
    void doNotLogHup() { logHup_ = false; }
    string reventsToString() const;
    string eventsToString() const;
    int index() const {return index_;}
    void set_index(int idx) { index_ = idx; }
    EventLoop* ownerLoop() { return loop_; }
    void remove();
private:
    static string eventsToString(int fd, int ev);
    void update();
    void handleEventWithGuard(Timestamp receiveTime);
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;
    bool logHup_;
    int index_;
    bool eventHandling_;
    bool addedToLoop_;
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

};
}
}