#ifndef HTTP_U_REQUEST_HPP
#define HTTP_U_REQUEST_HPP

#include <boost/beast/http.hpp>

#include "u_url.hpp"
#include "u_header.hpp"

namespace uhttp {

namespace http = boost::beast::http;

template<typename Body>
class Request {
public:
    Request() = default;

    std::string Host() {
        return url.GetHost();
    }

    size_t Port() {
        return url.GetPort();
    }

    Header<http::request<Body>>& Header() {
        return header;
    }

    Url<http::request<Body>>& Url() {
        return url;
    }

    http::request<Body>& Native() {
        return request;
    }

private:
    http::request<Body> request;
    Header<http::request<Body>> header{request};
    Url<http::request<Body>> url{request};
};

using GetRequest = Request<http::empty_body>;

}

#endif
