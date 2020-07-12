#ifndef HTTP_U_METHOD_OPTION_HPP
#define HTTP_U_METHOD_OPTION_HPP

namespace uhttp {

enum class Method {
    GET,
    POST
};

template<typename Request>
class MethodOption {
public:
    MethodOption(Method method) : _method(method) {
    }

    void SetOption(Request& req) {
        req.method(GetMethod(_method));
    }

private:
    http::verb GetMethod(Method method) {
        switch (method) {
            case Method::GET:
                return http::verb::get;
            case Method::POST:
                return http::verb::post;
            default:
                return http::verb::get;
        }
    }

private:
    Method _method;
};

}

#endif
