
#include "http/request.h"

using namespace StructBX::HTTP;

Request::Request()
{
    http_server_request_ = std::nullopt;
    http_server_response_ = std::nullopt;
}

void Request::AddHeader_(std::string name, std::string value)
{
    headers_.push_back(Header(name, value));
}

void Request::AddHeader_(HTTP::Header header)
{
    headers_.push_back(header);
}

void Request::AddCookie_(std::string name, std::string value)
{
    cookies_.push_back(Cookie(name, value));
}

void Request::AddCookie_(HTTP::Cookie cookie)
{
    cookies_.push_back(cookie);
}

void Request::SetupRequest_(Net::HTTPServerRequest& request)
{
    http_server_request_.emplace(&request);
}

void Request::SetupResponse_(Net::HTTPServerResponse& response)
{
    http_server_response_.emplace(&response);
}

void Request::SetupHeaders_()
{
    for(auto& header : headers_)
        http_server_response_.value()->add(header.name, header.value);
}

void Request::SetupCookies_()
{
    for(auto& cookie : cookies_)
    {
        if(cookie.name == "")
        {
            http_server_response_.value()->addCookie(cookie.cookie);
        }
        else
        {
            HTTPCookie poco_cookie(cookie.name, cookie.value);
            http_server_response_.value()->addCookie(poco_cookie);
        }
    }
}