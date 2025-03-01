
#ifndef STRUCTBX_HTTP_REQUEST
#define STRUCTBX_HTTP_REQUEST


#include <vector>
#include <optional>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPCookie.h>
#include <Poco/Net/NameValueCollection.h>


namespace StructBX
{
    namespace HTTP
    {
        class Header;
        class Cookie;
        class Request;
    }
}

using namespace Poco;
using namespace Poco::Net;



class StructBX::HTTP::Header
{
    public:
        Header(std::string name, std::string value) : 
            name(name)
            ,value(value)
        {}
        virtual ~Header(){}

        std::string name;
        std::string value;
};

class StructBX::HTTP::Cookie
{
    public:
        Cookie(std::string name, std::string value) : 
            name(name)
            ,value(value)
        {}
        Cookie(Net::HTTPCookie cookie) : 
            name("")
            ,value("")
            ,cookie(cookie)
        {}
        virtual ~Cookie(){}

        std::string name;
        std::string value;
        Net::HTTPCookie cookie;
};

class StructBX::HTTP::Request
{
    public:
        using HTTPServerRequestPtr = std::optional<HTTPServerRequest*>;
        using HTTPServerResponsePtr = std::optional<HTTPServerResponse*>;

        Request();
        
        HTTPServerRequestPtr& get_http_server_request()
        {
            auto& var = http_server_request_;
            return var;
        }
        HTTPServerResponsePtr& get_http_server_response()
        {
            auto& var = http_server_response_;
            return var;
        }
        std::vector<HTTP::Header> get_headers()
        {
            auto& var = headers_;
            return var;
        }
        std::vector<HTTP::Cookie> get_cookies()
        {
            auto& var = cookies_;
            return var;
        }
        
        void AddHeader_(std::string name, std::string value);
        void AddHeader_(HTTP::Header header);
        void AddCookie_(std::string name, std::string value);
        void AddCookie_(HTTP::Cookie cookie);

    protected:
        void SetupRequest_(Net::HTTPServerRequest& request);
        void SetupResponse_(Net::HTTPServerResponse& response);
        void SetupHeaders_();
        void SetupCookies_();

    private:
        HTTPServerRequestPtr http_server_request_;
        HTTPServerResponsePtr http_server_response_;
        std::vector<HTTP::Header> headers_;
        std::vector<HTTP::Cookie> cookies_;
};

#endif // STRUCTBX_HTTP_REQUEST
