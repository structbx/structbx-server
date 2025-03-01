
#ifndef STRUCTBX_QUERY_RESULTS
#define STRUCTBX_QUERY_RESULTS


#include <vector>

#include "query/row.h"


namespace StructBX
{
    namespace Query
    {
        class Results;
    }
}


class StructBX::Query::Results : public std::vector<Row::Ptr>
{
    public:
        using Ptr = std::shared_ptr<Results>;

        Results();

        Field::Ptr First_();
        Field::Ptr Last_();
        Field::Ptr ExtractField_(std::size_t row, std::size_t column);
        Field::Ptr ExtractField_(Field::Position& field_position);
        Row::Ptr AddRow_();
};

#endif // STRUCTBX_QUERY_RESULTS
