
#ifndef STRUCTBX_TOOLS_HMACTOOL
#define STRUCTBX_TOOLS_HMACTOOL

#include <string>

#include "Poco/HMACEngine.h"
#include "Poco/SHA1Engine.h"


namespace NAF
{
    namespace Tools
    {
        class HMACTool;
    }
}

using namespace NAF;

class NAF::Tools::HMACTool
{
    public:
        HMACTool();

        std::string Encode_(std::string text_to_encode);
};

#endif //STRUCTBX_TOOLS_HMACTOOL