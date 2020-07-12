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

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;

template<typename ReqBody, typename RespBody>
class Session : public std::enable_shared_from_this<Session<ReqBody, RespBody>> {
    using ReqPtr = std::shared_ptr<Request<ReqBody>>;
    using RespPtr = std::shared_ptr<Response<RespBody>>;
    using tcp = boost::asio::ip::tcp;

public:
    Session(net::io_context& io) 
        : resolver(net::make_strand(io))
        , stream(net::make_strand(io))
        , milliSecond(10000) {
    }

    void SetTimeOut(std::chrono::milliseconds& sec) {
        milliSecond = sec;
    }

    void SetContext(ReqPtr req, RespPtr rsp, 
        std::function<void(RespPtr)>&& responseFn,
        std::function<void(int, std::string)>&& errorFn) {
        request = req;
        response = rsp;
        responseCallback = responseFn;
        errorCallback = errorFn;
    }

    void Run() {
        tcp::resolver::query query(request->Host(), request->Port());
        resolver.async_resolve(query, 
            beast::bind_front_handler(&Session::OnResolve, this->shared_from_this()));
    }

    void OnResolve(beast::error_code ec, tcp::resolver::results_type results) {
        if (ec.failed()) {
            errorCallback(ec.value(), ec.message());
            return;
        }

        stream.expires_after(milliSecond);
        stream.async_connect(results, beast::bind_front_handler(
            &Session::OnConnect, this->shared_from_this())
        );
    }

    void OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
        if (ec.failed()) {
            errorCallback(ec.value(), ec.message());
            return;
        }

        stream.expires_after(milliSecond);
        http::async_write(stream, request->Native(), beast::bind_front_handler(
            &Session::OnWrite, this->shared_from_this()));
    }

    void OnWrite(beast::error_code ec, size_t) {
        if (ec.failed()) {
            errorCallback(ec.value(), ec.message());
            return;
        }
        
        http::async_read(stream, buffer, response->Native(), beast::bind_front_handler(
                &Session::OnRead, this->shared_from_this()));
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

    std::chrono::milliseconds milliSecond;
};

}

#endif
