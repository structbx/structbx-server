
#ifndef STRUCTBX_QUERY_FIELD
#define STRUCTBX_QUERY_FIELD


#include <string>

#include "tools/dvalue.h"


namespace StructBX
{
    namespace Query
    {
        class Field;
    }
}


class StructBX::Query::Field
{
    public:
        using Ptr = std::shared_ptr<Field>;

        struct Position
        {
            Position(std::size_t row, std::size_t column) : row(row), column(column) {}

            std::size_t row;
            std::size_t column;
        };

        Field();
        Field(std::string column_name, Tools::DValue::Ptr value);

        std::string get_column_name() const { return column_name_;}
        Tools::DValue::Ptr get_value() const {return value_;}
        Tools::DValue::Ptr& get_value()
        {
            auto& var = value_;
            return var;
        }

        void set_column_name(std::string column_name) { column_name_ = column_name;}
        void set_value(Tools::DValue::Ptr value) { value_ = value;}

        bool IsNull_();
        std::string ToString_();
        std::string& String_();
        float& Float_();
        bool& Bool_();
        int& Int_();

    private:
        bool is_null_;
        std::string column_name_;
        Tools::DValue::Ptr value_;
};


#endif // STRUCTBX_QUERY_FIELD
