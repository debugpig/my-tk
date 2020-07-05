#ifndef HTTP_U_HEADER_HPP
#define HTTP_U_HEADER_HPP

#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>

#include "u_url.hpp"

namespace uhttp {

enum class Version {
    HTTP_1_0,
    HTTP_1_1
};

enum class Method {
    GET,
    POST
};

template<typename Request>
class Header {
    using http = boost::beast::http;

public:
    Header(Request& req) : request{req} {
    }

    Header& SetVersion(Version ver) {
        request.version(ver == Version::HTTP_1_0 ? 10 : 11);
        return *this;
    }

    Header& SetMethod(Method method) {
        request.method(GetMethod(method));
        return *this;
    }

    template<typename Option>
    Header& SetOption(Option& option) {
        option.SetOption(request);
        return *this;
    }

private:
    http::verb GetMethod(Method method) {
        switch (method) {
            case Method::Get:
                return http::verb::get;
            case Method::POST:
                return http::verb::post;
            default:
                return http::verb::get;
        }
    }

private:
    Request& request;
};

}

#endif
