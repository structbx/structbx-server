
#ifndef STRUCTBX_FILES_FILEMANAGER
#define STRUCTBX_FILES_FILEMANAGER


#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <fstream>
#include <memory>
#include <iostream>
#include <exception>
#include <stdexcept>

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/URI.h"
#include "Poco/DateTime.h"
#include "Poco/Random.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/CountingStream.h"
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>

#include "files/file_properties.h"
#include "files/file.h"
#include "tools/settings_manager.h"
#include "tools/output_logger.h"


namespace StructBX
{
    namespace Files
    {
        enum class OperationType;
        enum class FileType;
        class FileManager;
    }
}

using namespace Poco;
using namespace Poco::Net;


enum class StructBX::Files::OperationType
{
    kDownload
    ,kUpload
    ,kDelete
};


class StructBX::Files::FileManager: public Net::PartHandler
{
    public:
        using Ptr = std::shared_ptr<FileManager>;

        FileManager();
        FileManager(FileManager& file_manager);
        FileManager(OperationType operation_type);
        ~FileManager();

        std::map<std::string, Files::FileProperties>& get_supported_files()
        {
            auto& var = supported_files_;
            return var;
        }
        OperationType get_operation_type() const{return operation_type_;}
        std::string get_directory_base() const{return directory_base_;}
        std::string get_directory_for_temp_files() const{return directory_for_temp_files_;}
        JSON::Object::Ptr& get_result()
        {
            auto& var = result_;
            return var;
        }
        std::vector<Files::File>& get_files()
        {
            auto& var = files_;
            return var;
        }

        void set_operation_type(OperationType operation_type){operation_type_ = operation_type;}
        void set_directory_base(std::string directory_base) { directory_base_ = directory_base;}
        void set_directory_for_temp_files(std::string directory_for_temp_files) { directory_for_temp_files_ = directory_for_temp_files;}

        void handlePart(const MessageHeader& header, std::istream& stream) override;
        std::string GenerateName_(std::string name);
        bool CheckFile_(Files::File& current_file);
        bool CheckFiles_();
        bool IsSupported_();
        bool IsSupported_(Files::File& file);
        void DownloadFile_(std::ostream& out_response);
        void UploadFile_();
        void RemoveFile_();
        void AddSupportedFile_(std::string extension, Files::FileProperties file_properties);
        void AddBasicSupportedFiles_();
        Files::File CreateTempFile_(std::string uri);
        Files::File CreateTempFileFromAddress_(std::string address);
        void ProcessContentLength_();
        bool VerifyMaxFileSize_();
        bool ChangePathAndFilename_(Files::File& file, std::string directory, bool change_filename = true);

    protected:
        void ProcessFiles_(Files::File& file, Files::FileProperties properties);
        void ProcessFileType_();
        std::string SplitHeaderValue_(const MessageHeader& header, std::string header_name, std::string parameter);
        std::size_t ReplaceText_(std::string& inout, std::string what, std::string with);

    private:
        std::map<std::string, Files::FileProperties> supported_files_;
        OperationType operation_type_;
        std::string directory_base_;
        std::string directory_for_temp_files_;
        JSON::Object::Ptr result_;
        std::vector<Files::File> files_;
};

#endif // STRUCTBX_FILES_FILEMANAGER
