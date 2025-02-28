
#ifndef STRUCTBX_TOOLS_HMACTOOL
#define STRUCTBX_TOOLS_HMACTOOL

#include <string>

#include "Poco/HMACEngine.h"
#include "Poco/SHA1Engine.h"


namespace StructBX
{
    namespace Tools
    {
        class HMACTool;
    }
}


class StructBX::Tools::HMACTool
{
    public:
        HMACTool();

        std::string Encode_(std::string text_to_encode);
};

#endif //STRUCTBX_TOOLS_HMACTOOL