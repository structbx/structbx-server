
#ifndef STRUCTBX_TOOLS_IDCHECKER
#define STRUCTBX_TOOLS_IDCHECKER

#include <list>
#include <string>
#include <algorithm>


namespace StructBX
{
    namespace Tools
    {
        class IDChecker;
    }
}

using namespace StructBX;
class StructBX::Tools::IDChecker
{
    public:
        IDChecker();

        bool Check_(std::string id);
        bool CheckNumbers_(std::string id);
        bool CheckEmail_(std::string id);

        std::list<char> allowed_characters_;
        std::list<char> allowed_characters_numbers_;
        std::list<char> allowed_characters_email_;

        std::string id_;
};

#endif //STRUCTBX_TOOLS_IDCHECKER