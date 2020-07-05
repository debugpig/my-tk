
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>

#include <http/u_request.hpp>
#include <http/u_response.hpp>
#include <http/u_session.hpp>
#include <http/u_header.hpp>
#include <http/u_url.hpp>

using namespace std;
namespace http = boost::beast::http;
namespace net = boost::asio; 
namespace fs = boost::filesystem;

int main(int argc, char** argv) {

    std::shared_ptr<uhttp::GetRequest> request = std::make_shared<uhttp::GetRequest>();
    request->Header().SetVersion(uhttp::Version::HTTP_1_1)
                     .SetMethod(uhttp::Method::GET);
    request->Url().SetHost("gitee.com").SetPath("api/v5/gists")
                  .AddParam("access_token", "2fe7e25ca43c607d127c8a3d650f72e7").SetTarget();

    std::shared_ptr<uhttp::JsonResponse> response = std::make_shared<uhttp::JsonResponse>();

    net::io_context io;
    uhttp::Session<http::empty_body, http::string_body> session(io);
    session.SetContext(request, response, [](std::shared_ptr<uhttp::JsonResponse> response) {
        cout << *response << endl;
    }, [] (int code, std::string errMsg) {
        cout << "error: " << code << ", reson: " << errMsg << endl;
    });

    session.Run();

    io.run();
    return 0;
}