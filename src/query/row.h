
#ifndef STRUCTBX_QUERY_ROW
#define STRUCTBX_QUERY_ROW


#include <vector>

#include "query/field.h"
#include "tools/dvalue.h"


namespace StructBX
{
    namespace Query
    {
        class Row;
    }
}


class StructBX::Query::Row : public std::vector<Field::Ptr>
{
    public:
        using Ptr = std::shared_ptr<Row>;

        Row();

        Field::Ptr ExtractField_(std::string column_name);
        Field::Ptr AddField_(std::string column_name, Tools::DValue::Ptr value);
};

#endif // STRUCTBX_QUERY_ROW
