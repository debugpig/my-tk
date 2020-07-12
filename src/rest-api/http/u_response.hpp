#ifndef HTTP_U_RESPONSE_HPP
#define HTTP_U_RESPONSE_HPP

#include <boost/beast/http.hpp>

namespace uhttp {

namespace http = boost::beast::http;

template<typename Body>
class Response {
public:
    Response() = default;

    http::response<Body>& Native() {
        return response;
    }


private:
    http::response<Body> response;
};

using JsonResponse = Response<http::string_body>;

}

#endif
