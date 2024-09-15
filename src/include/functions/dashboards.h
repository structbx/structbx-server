
#ifndef STRUCTBI_FUNCTIONS_DASHBOARDS
#define STRUCTBI_FUNCTIONS_DASHBOARDS

#include "function_data.h"

class Dashboards : public FunctionData
{
    public:
        Dashboards(FunctionData& function_data);

    protected:
        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();
};

#endif //STRUCTBI_FUNCTIONS_DASHBOARDS