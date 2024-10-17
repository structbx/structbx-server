
#ifndef STRUCTBI_FUNCTIONS_SPACES
#define STRUCTBI_FUNCTIONS_SPACES


#include "function_data.h"
#include "tools/base64_tool.h"
#include "actions/spaces_data.h"

using namespace StructBI;

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
        Actions::SpacesData spaces_data_;

};

#endif //STRUCTBI_FUNCTIONS_SPACES