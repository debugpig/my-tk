
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>

#include <http/u_request.hpp>
#include <http/u_response.hpp>
#include <http/u_session.hpp>
#include <http/u_url.hpp>
#include <http/option/u_version_option.hpp>
#include <http/option/u_method_option.hpp>

using namespace std;
namespace http = boost::beast::http;
namespace net = boost::asio; 
namespace fs = boost::filesystem;

int main(int argc, char** argv) {
    string host = "gitee.com";
    string path = "api/v5/gists";
    string tokenParaKey = "access_token";
    string tokenParaValue = "2fe7e25ca43c607d127c8a3d650f72e7";
    uhttp::Url<uhttp::GetRequestNavive> url;
    url.SetHost(host).SetPath(path).AddParam(tokenParaKey, tokenParaValue);

    shared_ptr<uhttp::GetRequest> request = make_shared<uhttp::GetRequest>();
    uhttp::VersionOption<uhttp::GetRequestNavive> version(uhttp::Version::HTTP_1_1);
    uhttp::MethodOption<uhttp::GetRequestNavive> method(uhttp::Method::GET);
    request->SetUrl(std::move(url)).SetOption(version, method);

    shared_ptr<uhttp::JsonResponse> response = make_shared<uhttp::JsonResponse>();

    net::io_context io;
    shared_ptr<uhttp::Session<http::empty_body, http::string_body>> session = make_shared<uhttp::Session<http::empty_body, http::string_body>>(io);
    session->SetContext(request, response, [](shared_ptr<uhttp::JsonResponse> response) {
        //cout << *response << endl;
        cout << "get response" << endl;
    }, [] (int code, string errMsg) {
        cout << "error: " << code << ", reson: " << errMsg << endl;
    });

    session->Run();

    io.run();
    return 0;
}