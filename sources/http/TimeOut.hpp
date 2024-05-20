#include <ctime>
#ifndef TIMOUT_HPP
#define TIMOUT_HPP

// default connection time out, this needs to come from the config
static const time_t DEFAULT_TIMEOUT(10);

struct TimeOut {
public:
    TimeOut() : lastEvent(time(0)) {};
    void update_time() { lastEvent = time(0); }
    bool is_timeout() { return (time(0) - lastEvent) > timeOut; };

private:
    const static time_t timeOut = DEFAULT_TIMEOUT;
    time_t              lastEvent;
};

#endif //  TIMOUT_HPP
