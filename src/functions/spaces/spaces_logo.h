
#ifndef STRUCTBI_FUNCTIONS_SPACESLOGO
#define STRUCTBI_FUNCTIONS_SPACESLOGO


#include "function_data.h"

class SpacesLogo : public FunctionData
{
    public:
        SpacesLogo(FunctionData& function_data);

    protected:
        void Read_();
        void Modify_();
};

#endif //STRUCTBI_FUNCTIONS_SPACESLOGO