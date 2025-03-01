
#include "http/methods.h"
#include <exception>
#include <stdexcept>

using namespace StructBX::HTTP;

Methods::Methods()
{
    MapMethods_();
}

EnumMethods Methods::GetMethod_(const std::string method)
{
    auto found = map_string_methods_.find(method);
    if(found != map_string_methods_.end())
        return found->second;
    else
    {
        return EnumMethods::kNULL;
        throw std::runtime_error("The HTTP method in GetMethod_ not exists.");
    }
}

const std::string Methods::GetMethod_(EnumMethods& method)
{
    auto found = map_enum_methods_.find(method);
    if(found != map_enum_methods_.end())
        return found->second;
    else
        return HTTP_GET;
}

void Methods::AddProcess_(const std::string method, MethodProcess process)
{
    MethodStruct method_struct;
    method_struct.method = GetMethod_(method);
    method_struct.process = process;
    process_.push_back(method_struct);
}

bool Methods::EvaluateProcess_(std::string request_method)
{
    for(auto& process : process_)
    {
        if(request_method == GetMethod_(process.method))
        {
            process.process(*this);
            return true;
        }
    }

    return false;
}
std::string Methods::IdentifyMethod_(std::string method)
{
    for(auto method_it: map_string_methods_)
    {
        if(method == method_it.first)
            return method;
    }

    return HTTP_GET;
}

void Methods::MapMethods_()
{
    map_string_methods_.insert(std::pair{HTTP_GET, EnumMethods::kHTTP_GET});
    map_string_methods_.insert(std::pair{HTTP_POST, EnumMethods::kHTTP_POST});
    map_string_methods_.insert(std::pair{HTTP_PUT, EnumMethods::kHTTP_PUT});
    map_string_methods_.insert(std::pair{HTTP_DEL, EnumMethods::kHTTP_DEL});
    map_string_methods_.insert(std::pair{HTTP_DELETE, EnumMethods::kHTTP_DEL});
    map_string_methods_.insert(std::pair{HTTP_OPTIONS, EnumMethods::kHTTP_OPTIONS});
    map_string_methods_.insert(std::pair{HTTP_HEAD, EnumMethods::kHTTP_HEAD});
    map_string_methods_.insert(std::pair{HTTP_PATCH, EnumMethods::kHTTP_PATCH});

    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_GET, HTTP_GET});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_POST, HTTP_POST});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_PUT, HTTP_PUT});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_DEL, HTTP_DEL});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_OPTIONS, HTTP_OPTIONS});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_HEAD, HTTP_HEAD});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_PATCH, HTTP_PATCH});
}