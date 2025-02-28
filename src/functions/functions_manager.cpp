
#include "functions/functions_manager.h"

using namespace StructBX::Functions;

FunctionsManager::FunctionsManager()
{

}

Function::Ptr FunctionsManager::AddFunction_(std::string endpoint, HTTP::EnumMethods method)
{
    Function::Ptr function = std::make_shared<Function>(endpoint, method);
    functions_.insert(std::pair(endpoint, function));
    return function;
}