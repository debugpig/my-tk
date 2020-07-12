#ifndef HTTP_U_URL_HPP
#define HTTP_U_URL_HPP

#include <string>
#include <cppcodec/base64_url.hpp>
#include <boost/beast/http.hpp>

namespace uhttp {

namespace http = boost::beast::http;
    
template<typename Request>
class Url {
    using base64_url = cppcodec::base64_url;

public:
    Url() = default;
    Url& SetHost(std::string& host, std::string port = "80") {
        _host = host;
        _port = port;
        return *this;
    }

    Url& SetPath(std::string& path) {
        if (path.empty() || path[0] != '/') {
            _path.push_back('/');
        }
        _path.append(path);
        return *this;
    }

    Url& AddParam(std::string& key, std::string& value) {
        _parameter.push_back(_parameter.empty() ? '?' : '&');
        _parameter.append(key).append("=").append(value);
        //_parameter.append(base64_url::encode(key)).append("=").append(base64_url::encode(value));
        return *this;
    }

    void SetOption(Request& req) {
        req.set(http::field::host, _host);
        req.target(_path + _parameter);
    }

    const std::string& GetHost() {
        return _host;
    }

    const std::string& GetPort() {
        return _port;
    }

private:
    std::string _host;
    std::string _port;
    std::string _path;
    std::string _parameter;
};

}

#endif
