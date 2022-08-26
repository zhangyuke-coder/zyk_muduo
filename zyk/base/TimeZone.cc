
#include "TimeZone.h"

#include "noncopyable.h"
#include <vector>
#include <stdio.h>
#include <assert.h>
#include "Types.h"
#include <stdint.h>
#include <algorithm>
namespace zyk {
namespace detail {

struct Transition {
    
time_t gmttime;
time_t localtime;
int localtimeIdx;
Transition (time_t t, time_t l, int localIdx) :
    gmttime(t),
    localtime(l),
    localtimeIdx(localIdx)
{}
};
struct Comp {
    bool compareGmt;
    Comp(bool gmt):
        compareGmt(gmt)
    {}
    bool operator()(const Transition& lhs, const Transition& rhs) const{
        if(compareGmt) {
            return lhs.gmttime < rhs.gmttime;
        } else {
            return lhs.localtime < rhs.localtime;
        }
    }


    bool equal(const Transition& lhs, const Transition& rhs) const {
        if (compareGmt)
            return lhs.gmttime == rhs.gmttime;
        else
            return lhs.localtime == rhs.localtime;
    }
};


struct Localtime
{
  time_t gmtOffset;
  bool isDst;
  int arrbIdx;

  Localtime(time_t offset, bool dst, int arrb)
    : gmtOffset(offset), isDst(dst), arrbIdx(arrb)
  { }
};


inline void fillHMS(unsigned seconds, struct tm* utc) {
    utc->tm_sec = seconds % 60;
    unsigned minutes = seconds / 60;
    utc->tm_min = minutes % 60;
    utc->tm_hour = minutes / 60;
}

const int kSecondsPerDay = 24 * 60 * 60;


}
}
using namespace zyk;
using namespace std;
struct TimeZone::Data {
    vector<detail::Transition> transitions;
    vector<detail::Localtime> localtimes;
    vector<string> names;
    string abbreviation;
};

namespace zyk {
namespace detail {

class File : noncopyable {
public:
    File(const char* file) :
        fp_(::fopen(file, "rb"))
    {}
    ~File() {
        if(fp_) {
            ::fclose(fp_);
        }
    }

    bool vaild() const {
        return fp_;

    }

    string readBytes(int n) {
        char buf[n];
        ssize_t nr = ::fread(buf, 1, n, fp_);
        if(nr != n) {
            throw logic_error("no enough data");
        }
        return string(buf, n);
        
    }

    int32_t readInt32() {
        int32_t x = 0;


        ssize_t nr = ::fread(&x, 1, sizeof(int32_t), fp_);
        if(nr != sizeof(int32_t)) {
            throw logic_error("bad int32_t data");

        }
        return be32toh(x);
    }

    uint8_t readUInt8() {
        uint8_t x = 0;
        ssize_t nr = fread(&x, 1, sizeof(uint8_t), fp_);
        if(nr != sizeof(uint8_t)) {
            throw logic_error("bad uint8_t data");
        }
        return x;
    }

    

private:
    FILE* fp_;
};

bool readTimeZoneFile(const char* zonefile, struct TimeZone::Data* data) {
        File f(zonefile);
        if(f.vaild()) {
            try {
                string head = f.readBytes(4);
                if (head != "TZif")
                    throw logic_error("bad head");
                string version = f.readBytes(1);
                f.readBytes(15); 


                int32_t isgmtcnt = f.readInt32();
                int32_t isstdcnt = f.readInt32();
                int32_t leapcnt = f.readInt32();
                int32_t timecnt = f.readInt32();
                int32_t typecnt = f.readInt32();
                int32_t charcnt = f.readInt32();
                vector<int32_t> trans;
                vector<int> localtimes;
                trans.reserve(timecnt);
                for (int i = 0; i < timecnt; ++i)
                {
                    trans.push_back(f.readInt32());
                }
                for (int i = 0; i < timecnt; ++i)
                {
                    uint8_t local = f.readUInt8();
                    localtimes.push_back(local);
                }

                for (int i = 0; i < typecnt; ++i)
                {
                    int32_t gmtoff = f.readInt32();
                    uint8_t isdst = f.readUInt8();
                    uint8_t abbrind = f.readUInt8();

                    data->localtimes.push_back(Localtime(gmtoff, isdst, abbrind));
                }

                for (int i = 0; i < timecnt; ++i)
                {
                int localIdx = localtimes[i];
                time_t localtime = trans[i] + data->localtimes[localIdx].gmtOffset;
                data->transitions.push_back(Transition(trans[i], localtime, localIdx));
                }
                data->abbreviation = f.readBytes(charcnt);


            } catch(logic_error& e) {
                fprintf(stderr, "%s\n", e.what());
            }
        }
        return true;
    }
    const Localtime* findLocaltime(const TimeZone::Data& data, Transition sentry, Comp comp)
    {
        const Localtime* local = NULL;

        if (data.transitions.empty() || comp(sentry, data.transitions.front()))
        {
            // FIXME: should be first non dst time zone
            local = &data.localtimes.front();
        }
        else
        {
            vector<Transition>::const_iterator transI = lower_bound(data.transitions.begin(),
                                                                    data.transitions.end(),
                                                                    sentry,
                                                                    comp);
            if (transI != data.transitions.end())
            {
            if (!comp.equal(sentry, *transI))
            {

                --transI;
            }
            local = &data.localtimes[transI->localtimeIdx];
            }
            else
            {
            // FIXME: use TZ-env
            local = &data.localtimes[data.transitions.back().localtimeIdx];
            }
        }

        return local;
    }
}
}



TimeZone::TimeZone(const char* zonefile)
  : data_(new TimeZone::Data)
{
  if (!detail::readTimeZoneFile(zonefile, data_.get()))
  {
    data_.reset();
  }
}

TimeZone::TimeZone(int eastOfUtc, const char* name) 
    : data_(new TimeZone::Data) {
        data_->localtimes.push_back(detail::Localtime(eastOfUtc, false, 0));
        data_->abbreviation = name;
    }

struct tm TimeZone::toLocalTime(timer_t secondsSinceEpoch) const {
    struct tm localTime;
    memZero(&localTime, sizeof(localTime));
    assert(data_ != NULL);
    

}