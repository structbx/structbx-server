
#ifndef STRUCTBI_FUNCTIONS_SPACESUSERS
#define STRUCTBI_FUNCTIONS_SPACESUSERS


#include "function_data.h"

class SpacesUsers : public FunctionData
{
    public:
        SpacesUsers(FunctionData& function_data);
    
    protected:
        void Read_();
};

#endif //STRUCTBI_FUNCTIONS_SPACESUSERS