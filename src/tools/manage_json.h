
#ifndef STRUCTBX_TOOLS_MANAGEJSON
#define STRUCTBX_TOOLS_MANAGEJSON


#include <istream>
#include <string>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/StreamCopier.h>

using namespace Poco;


namespace StructBX
{
    namespace Tools
    {
        class ManageJSON;
    }
}


class StructBX::Tools::ManageJSON
{
    public:
        ManageJSON();
        ~ManageJSON();

        JSON::Array::Ptr get_json_array() const { return json_array_; }
        JSON::Object::Ptr get_json_object() const { return json_object_; }
        JSON::Array::Ptr& get_json_array()
        {
            auto& var = json_array_;
            return var;
        }
        JSON::Object::Ptr& get_json_object()
        {
            auto& var = json_object_;
            return var;
        }
        JSON::Array::Ptr& get_data()
        {
            auto& var = data_;
            return var;
        }

        void set_json_array(JSON::Array::Ptr json_array) { json_array_ = json_array; }
        void set_json_object(JSON::Object::Ptr json_object) { json_object_ = json_object; }
        void set_data(JSON::Array::Ptr data) { data_ = data; }

        void Parse_(std::string& string_to_parse);
        JSON::Object::Ptr ExtractObject_(Dynamic::Var& object);
        JSON::Array::Ptr ExtractArray_(Dynamic::Var& object);

    private:
        JSON::Array::Ptr json_array_;
        JSON::Object::Ptr json_object_;
        JSON::Array::Ptr data_;
};

#endif // STRUCTBX_TOOLS_MANAGEJSON
