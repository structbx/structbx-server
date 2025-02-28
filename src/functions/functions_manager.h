
#ifndef STRUCTBX_FUNCTIONS_FUNCTIONS_MANAGER
#define STRUCTBX_FUNCTIONS_FUNCTIONS_MANAGER


#include <string>
#include <map>

#include "functions/function.h"
#include "http/methods.h"


namespace StructBX
{
    namespace Functions
    {
        class FunctionsManager;
    }
}


class StructBX::Functions::FunctionsManager
{
    public:
        using FunctionsMap = std::map<std::string, Function::Ptr>;

        FunctionsManager();

        FunctionsMap& get_functions()
        {
            auto& var = functions_;
            return var;
        }

        Function::Ptr AddFunction_(std::string endpoint, HTTP::EnumMethods method);

    private:
        FunctionsMap functions_;
};

#endif // STRUCTBX_FUNCTIONS_FUNCTIONS_MANAGER
