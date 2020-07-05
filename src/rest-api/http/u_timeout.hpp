#ifndef HTTP_U_TIMEOUT_HPP
#define HTTP_U_TIMEOUT_HPP

#include <chrono>

namespace http {

class TimeOut {
public:
    TimeOut(std::chrono::microseconds&& ms) : mircoSecond(ms) {}

private:
    std::chrono::microseconds mircoSecond;
};

}

#endif
