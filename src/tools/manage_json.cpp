
#include "tools/manage_json.h"

using namespace StructBX::Tools;

ManageJSON::ManageJSON() :
    json_array_(new JSON::Array())
    ,json_object_(new JSON::Object())
{

}

ManageJSON::~ManageJSON()
{

}

void ManageJSON::Parse_(std::string& string_to_parse)
{
    JSON::Parser parser;

    Dynamic::Var var_tmp(parser.parse(string_to_parse));

    if(var_tmp.isArray())
        json_array_ = var_tmp.extract<JSON::Array::Ptr>();
    else
        json_object_ = var_tmp.extract<JSON::Object::Ptr>();
}

JSON::Object::Ptr ManageJSON::ExtractObject_(Dynamic::Var& object)
{
    if(object.isEmpty() || object.isArray())
        return JSON::Object::Ptr();

    return object.extract<JSON::Object::Ptr>();
}

JSON::Array::Ptr ManageJSON::ExtractArray_(Dynamic::Var& object)
{
    auto final = JSON::Array::Ptr();

    if(object.isEmpty() || !object.isArray())
        return final;

    final = object.extract<JSON::Array::Ptr>();
    return final;
}
