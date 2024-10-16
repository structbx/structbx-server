
#ifndef STRUCTBI_TOOLS_BASE64TOOL
#define STRUCTBI_TOOLS_BASE64TOOL

#include <string>
#include <sstream>

#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"


class Base64Tool
{
    public:
        Base64Tool(){}

        std::string Encode_(std::string text_to_encode)
        {
            std::stringstream str_enc("");

            Poco::Base64Encoder encoder(str_enc);
            encoder << text_to_encode;
            encoder.close();

            return str_enc.str();
        }

        std::string Decode_(std::string text_to_decode)
        {
            std::stringstream str_dec(text_to_decode);
            Poco::Base64Decoder decoder(str_dec);
            std::stringstream out;
	        Poco::StreamCopier::copyStream(decoder, out);
            return out.str();
        }
};

#endif //STRUCTBI_TOOLS_BASE64TOOL