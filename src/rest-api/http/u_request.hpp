#ifndef HTTP_U_REQUEST_HPP
#define HTTP_U_REQUEST_HPP

#include <boost/beast/http.hpp>

#include "u_url.hpp"

namespace uhttp {

namespace http = boost::beast::http;

template<typename Body>
class Request {
public:
    Request() {};

    template<typename Option, typename ...Options>
    void SetOption(Option& option, Options&... options) {
        option.SetOption(_request);
        SetOption(options...);
    }

    template<typename Option>
    void SetOption(Option& option) {
        option.SetOption(_request);
    }

    Request& SetUrl(Url<http::request<Body>>&& urlOpt) {
        _url = urlOpt;
        _url.SetOption(_request);
        return *this;
    }

    const std::string& Host() {
        return _url.GetHost();
    }

    const std::string& Port() {
        return _url.GetPort();
    }

    http::request<Body>& Native() {
        return _request;
    }

private:
    http::request<Body> _request;
    Url<http::request<Body>> _url;
};

using GetBody = http::empty_body;
using GetRequest = Request<http::empty_body>;
using GetRequestNavive = http::request<http::empty_body>;

}

#endif
