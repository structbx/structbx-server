
#ifndef STRUCTBX_TOOLS_BASE64TOOL
#define STRUCTBX_TOOLS_BASE64TOOL

#include <string>
#include <sstream>

#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"


namespace StructBX
{
    namespace Tools
    {
        class Base64Tool;
    }
}

class StructBX::Tools::Base64Tool
{
    public:
        Base64Tool();

        std::string Encode_(std::string text_to_encode);

        std::string Decode_(std::string text_to_decode);
};

#endif //STRUCTBX_TOOLS_BASE64TOOL