
#include "files/file_properties.h"

using namespace StructBX::Files;

FileProperties::FileProperties(std::string content_type, bool binary, std::vector<std::string> other_extensions) :
    content_type_(content_type)
    ,binary_(binary)
    ,other_extensions_(other_extensions)
{

}

FileProperties::~FileProperties()
{

}
