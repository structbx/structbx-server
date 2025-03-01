
#ifndef STRUCTBX_FILES_FILEPROPERTIES
#define STRUCTBX_FILES_FILEPROPERTIES


#include <string>
#include <vector>


namespace StructBX
{
	namespace Files
	{
		class FileProperties;
	}
}


class StructBX::Files::FileProperties
{
	public:
		FileProperties(std::string content_type, bool binary, std::vector<std::string> other_extensions);
		~FileProperties();

		std::string get_content_type()
		{
			auto& var = content_type_;
			return var;
		}
		bool get_binary()
		{
			auto& var = binary_;
			return var;
		}
		std::vector<std::string> get_other_extensions()
		{
			auto& var = other_extensions_;
			return var;
		}

	private:
		std::string content_type_;
		bool binary_;
		std::vector<std::string> other_extensions_;
};

#endif // STRUCTBX_FILES_FILEPROPERTIES
