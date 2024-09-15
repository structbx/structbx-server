
#ifndef STRUCTBI_FUNCTIONS_REPORTS
#define STRUCTBI_FUNCTIONS_REPORTS

#include "function_data.h"

class ReportsGraphs : public FunctionData
{
    public:
        ReportsGraphs(FunctionData& function_data);
        
    protected:
        void Read_();
};

class ReportsParameters : public FunctionData
{
    public:
        ReportsParameters(FunctionData& function_data);
        
    protected:
        void Read_();
};

class Reports : public FunctionData
{
    public:
        Reports(FunctionData& function_data);
        
    protected:
        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        ReportsGraphs reports_graphs_;
        ReportsParameters reports_parameters_;
};

#endif //STRUCTBI_FUNCTIONS_REPORTS