
#ifndef STRUCTBI_FUNCTIONS_SPACES
#define STRUCTBI_FUNCTIONS_SPACES


#include "function_data.h"

class SpacesLogo : public FunctionData
{
    public:
        SpacesLogo(FunctionData& function_data);

    protected:
        void Read_();
        void Modify_();
};

class SpacesUsers : public FunctionData
{
    public:
        SpacesUsers(FunctionData& function_data);
    
    protected:
        void Read_();
};

class Spaces : public FunctionData
{
    public:
        Spaces(FunctionData& function_data);
        
    protected:
        void Read_();
        void ReadSpecific_();
        void Change_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        SpacesLogo spaces_logo_;
        SpacesUsers spaces_users_;
};

#endif //STRUCTBI_FUNCTIONS_SPACES