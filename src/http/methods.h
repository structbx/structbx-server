
#ifndef STRUCTBX_HTTP_METHODS
#define STRUCTBX_HTTP_METHODS

#include <map>
#include <string>
#include <functional>
#include <list>
#include <exception>


namespace StructBX
{
    namespace HTTP
    {
        const std::string HTTP_GET = "GET";
        const std::string HTTP_POST = "POST";
        const std::string HTTP_PUT = "PUT";
        const std::string HTTP_DEL = "DEL";
        const std::string HTTP_DELETE = "DELETE";
        const std::string HTTP_OPTIONS = "OPTIONS";
        const std::string HTTP_HEAD = "HEAD";
        const std::string HTTP_PATCH = "PATCH";
        enum class EnumMethods;
        class Methods;
    }
}


enum class StructBX::HTTP::EnumMethods
{
    kHTTP_GET
    ,kHTTP_POST
    ,kHTTP_PUT
    ,kHTTP_DEL
    ,kHTTP_OPTIONS
    ,kHTTP_HEAD
    ,kHTTP_PATCH
    ,kNULL
};

class StructBX::HTTP::Methods
{
    public:
        using MethodProcess = std::function<void(HTTP::Methods&)>;

        struct MethodStruct
        {
            HTTP::EnumMethods method;
            MethodProcess process;
        };

        Methods();
        virtual ~Methods(){}

        std::map<const std::string, EnumMethods>& get_map_string_methods()
        {
            auto& var = map_string_methods_;
            return var;
        }
        std::map<EnumMethods, const std::string>& get_map_enum_methods()
        {
            auto& var = map_enum_methods_;
            return var;
        }

        EnumMethods GetMethod_(const std::string method);
        const std::string GetMethod_(EnumMethods& method);
        void AddProcess_(const std::string method, MethodProcess process);
        bool EvaluateProcess_(std::string request_method);
        std::string IdentifyMethod_(std::string method);

    protected:
        void MapMethods_();

    private:
        std::map<const std::string, EnumMethods> map_string_methods_;
        std::map<EnumMethods, const std::string> map_enum_methods_;
        std::list<MethodStruct> process_;
};

#endif // STRUCTBX_HTTP_METHODS
