#ifndef HTTP_U_SESSION_HPP
#define HTTP_U_SESSION_HPP

#include <functional>
#include <memory>
#include <chrono>

#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>

#include "u_url.hpp"
#include "u_request.hpp"
#include "u_response.hpp"

namespace uhttp {

template<typename ReqBody, typename RespBody>
class Session : public std::enable_shared_from_this<Session<ReqBody, RespBody>> {
    using beast = boost::beast;
    using http = boost::beast::http;
    using net = boost::asio;
    using tcp = boost::asio::ip::tcp;

    using ReqPtr = std::shared_ptr<Request<ReqBody>>;
    using RespPtr = std::shared_ptr<Response<RespBody>>;

public:
    Session(net::io_context& io) 
        : resolver(net::make_strand(io))
        , stream(net::make_strand(io))
        , mircoSecond(10000) {
    }

    void SetTimeOut(std::chrono::microseconds& sec) {
        mircoSecond = sec;
    }

    void SetContext(ReqPtr req, RespPtr rsp, 
        std::function<void(RespPtr)>&& responseFn,
        std::function<void(int, std::string)&& errorFn) {
        request = req;
        response = rsp;
        responseCallback = responseFn;
        errorCallback = errorFn;
    }

    void Run() {
        resolver.async_resolve(request->Host(), request->Port(), 
            beast::bind_front_handler(&Session::OnResolve, shared_from_this()));
    }

    void OnResolve(beast::error_code ec, tcp::resolver::results_type results) {
        if (ec.failed()) {
            errorCallback(ec.value(), ec.message());
            return;
        }

        stream.expires_after(mircoSecond);
        stream.async_connect(results, beast::bind_front_handler(
            &Session::OnConnect, shared_from_this())
        );
    }

    void OnConnect(beast::error_code ec, tcp::resolver::results_type::endpointType) {
        if (ec.failed()) {
            errorCallback(ec.value(), ec.message());
            return;
        }

        stream.expires_after(mircoSecond);
        http::async_write(stream, request->Native(), beast::bind_front_handler(
            &Session::OnWrite, shared_from_this()));
    }

    void OnWrite(beast::error_code ec, size_t) {
        if (ec.failed()) {
            errorCallback(ec.value(), ec.message());
            return;
        }
        
        http::async_read(stream, buffer, response->Native(), beast::bind_front_handler(
                &Session::OnRead, shared_from_this()));
    }

    void OnRead(beast::error_code ec, size_t) {
        if (ec.failed()) {
            errorCallback(ec.value(), ec.message());
            return;
        }
        
        responseCallback(response);

        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        if(ec.failed() && ec != beast::errc::not_connected) {
            errorCallback(ec.value(), ec.message());
        }
    }

private:
    ReqPtr request;
    RespPtr response;
    std::function<void(RespPtr)> responseCallback;
    std::function<void(int, std::string)> errorCallback;

    tcp::resolver resolver;
    beast::tcp_stream stream;
    beast::flat_buffer buffer;

    std::chrono::microseconds mircoSecond;
};

}

#endif
