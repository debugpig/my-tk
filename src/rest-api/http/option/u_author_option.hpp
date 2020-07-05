#ifndef HTTP_U_AUTHOR_OPTION_HPP
#define HTTP_U_AUTHOR_OPTION_HPP

#include <boost/beast/http.hpp>
#include <cppcodec/base32_default_rfc4648.hpp>

namespace uhttp {

template<typename Request>
class AuthorizationOption {
    using http = boost::beast::http;
    using base64 = cppcodec::base32_rfc4648;
public:
    AuthorizationOption(std::string& name, std::string& password) {
        authorization = base64::encode(name+":"+password);
    }

    void SetOption(Request& req) {
        req.set(http::field::authorization, authorization);
    }

private:
    std::string authorization;
};

}

#endif
