
#ifndef STRUCTBI_FUNCTIONS_FORMS
#define STRUCTBI_FUNCTIONS_FORMS

#include "function_data.h"

class FormsData : public FunctionData
{
    public:
        FormsData(FunctionData& function_data);

    protected:
        void Read_();
};

class Forms : public FunctionData
{
    public:
        Forms(FunctionData& function_data);

    protected:
        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        FormsData forms_data_;
};

#endif //STRUCTBI_FUNCTIONS_FORMS