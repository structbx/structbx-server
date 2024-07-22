

#include <list>
#include <string>
#include <algorithm>


class IDChecker
{
    public:
        IDChecker();

        bool Check_(std::string& id);

        std::list<char> allowed_characters_;

        std::string id_;
};
