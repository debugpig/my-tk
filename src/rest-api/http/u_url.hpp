#ifndef HTTP_U_URL_HPP
#define HTTP_U_URL_HPP

#include <string>
#include <cppcodec/base64_url.hpp>
#include <boost/beast/http.hpp>

namespace uhttp {

template<typename Request>
class Url {
    using base64_url = cppcodec::base64_url;
    using http = boost::beast::http;

public:
    Url(Request& req) : request{req} {}

    Url& SetHost(std::string& host, size_t port = 80) {
        request.set(http::field::host, host);
        this->host = host;
        this->port = port;
        return *this;
    }

    Url& SetPath(std::string& path) {
        this->path = path;
        return *this;
    }

    Url& AddParam(std::string& key, std::string& value) {
        parameter.push_back(parameter.empty() ? '?' : '&');
        parameter.append(base64_url::encode(key)).append("=").append(base64_url::encode(value));
        return *this;
    }

    Url& SetTarget() {
        request.target(path + parameter);
        return *this;
    }

    std::string GetHost() {
        return host;
    }

    size_t GetPort() {
        return port;
    }

private:
    Request request;
    std::string host;
    size_t port;
    std::string path;
    std::string parameter;
};

}

#endif
