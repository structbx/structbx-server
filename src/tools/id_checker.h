
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

        std::list<char> allowed_characters_;

        std::string id_;
};

#endif //STRUCTBI_TOOLS_IDCHECKER