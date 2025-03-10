
#include "files/file.h"
#include "files/file_properties.h"

using namespace StructBX::Files;

StructBX::Files::File::File() :
    content_length_(0)
    ,content_type_("")
    ,name_("")
    ,filename_("")
    ,file_type_(FileType::kBinary)
    ,file_properties_("", true, {})
    ,requested_path_(new Path())
    ,requested_file_(new Poco::File(*requested_path_))
    ,tmp_file_(new Poco::File())
{

}

StructBX::Files::File::File(std::string name, std::string filename, std::string content_type, std::size_t content_length) :
    content_length_(content_length)
    ,content_type_(content_type)
    ,name_(name)
    ,filename_(filename)
    ,file_type_(FileType::kBinary)
    ,file_properties_("", true, {})
    ,requested_path_(new Path())
    ,requested_file_(new Poco::File(*requested_path_))
    ,tmp_file_(new Poco::File())
{

}
