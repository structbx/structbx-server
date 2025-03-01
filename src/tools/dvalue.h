
#ifndef STRUCTBX_TOOLS_DVALUE
#define STRUCTBX_TOOLS_DVALUE


#include <string>
#include <stdexcept>

#include <Poco/Dynamic/Var.h>
#include "Poco/DateTimeParser.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"

#include "tools/output_logger.h"


namespace StructBX
{
    namespace Tools
    {
        class DValue;
    }
}

using namespace Poco;


class StructBX::Tools::DValue
{
    public:
        using Ptr = std::shared_ptr<DValue>;

        enum class Type
        {
            kEmpty
            ,kString
            ,kInteger
            ,kFloat
            ,kBoolean
        };

        DValue();
        DValue(Poco::Dynamic::Var& value);
        DValue(std::string value_string);
        DValue(const char* value_string);
        DValue(int value_int);
        DValue(float value_float);
        DValue(bool value_bool);
        ~DValue(){}

        Type& get_type()
        {
            auto& var = type_;
            return var;
        }

        bool operator==(DValue& dvalue);
        bool operator!=(DValue& dvalue);
        bool operator<(DValue& dvalue);
        bool operator<=(DValue& dvalue);
        bool operator>(DValue& dvalue);
        bool operator>=(DValue& dvalue);
        bool TypeIsIqual_(Type row_value_type);
        std::string ToString_();
        std::string& String_();
        float& Float_();
        bool& Bool_();
        int& Int_();

    protected:
        std::string& get_value_string()
        {
            auto& var = value_string_;
            return var;
        }
        int& get_value_int()
        {
            auto& var = value_int_;
            return var;
        }
        float& get_value_float()
        {
            auto& var = value_float_;
            return var;
        }
        bool& get_value_bool()
        {
            auto& var = value_bool_;
            return var;
        }

        void Format_(Poco::Dynamic::Var& value);

    private:
        Type type_;

        std::string value_string_;
        int value_int_;
        float value_float_;
        bool value_bool_;

};


#endif // STRUCTBX_TOOLS_DVALUE
