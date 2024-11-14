#include "tools/id_checker.h"


Tools::IDChecker::IDChecker()
{
    allowed_characters_ = std::list<char>({
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S'
        , 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'
        , 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2'
        , '3', '4', '5', '6', '7', '8', '9', '_'});
    allowed_characters_numbers_ = std::list<char>({
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ','});
    allowed_characters_email_ = std::list<char>({
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S'
        , 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'
        , 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2'
        , '3', '4', '5', '6', '7', '8', '9', '_', '.', '@'});
}

bool Tools::IDChecker::Check_(std::string id)
{
    for (char c : id)
    {
        if (std::find(allowed_characters_.begin(), allowed_characters_.end(), c) == allowed_characters_.end())
        {
            return false;
        }
    }
    return true;
}

bool Tools::IDChecker::CheckNumbers_(std::string id)
{
    for (char c : id)
    {
        if (std::find(allowed_characters_numbers_.begin(), allowed_characters_numbers_.end(), c) == allowed_characters_numbers_.end())
        {
            return false;
        }
    }
    return true;
}
bool Tools::IDChecker::CheckEmail_(std::string id)
{
    for (char c : id)
    {
        if (std::find(allowed_characters_email_.begin(), allowed_characters_email_.end(), c) == allowed_characters_email_.end())
        {
            return false;
        }
    }
    return true;
}