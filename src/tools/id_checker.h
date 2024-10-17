
#ifndef STRUCTBI_TOOLS_IDCHECKER
#define STRUCTBI_TOOLS_IDCHECKER

#include <list>
#include <string>
#include <algorithm>


class IDChecker
{
    public:
        IDChecker();

        bool Check_(std::string id);
        bool CheckNumbers_(std::string id);

        std::list<char> allowed_characters_;
        std::list<char> allowed_characters_numbers_;

        std::string id_;
};

#endif //STRUCTBI_TOOLS_IDCHECKER