#ifndef HTTP_U_TIMEOUT_HPP
#define HTTP_U_TIMEOUT_HPP

#include <chrono>

namespace http {

class TimeOut {
public:
    TimeOut(std::chrono::milliseconds&& ms) : milliSecond(ms) {}

private:
    std::chrono::milliseconds milliSecond;
};

}

#endif
