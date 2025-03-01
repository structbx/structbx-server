
#ifndef STRUCTBX_SECURITY_USER
#define STRUCTBX_SECURITY_USER


#include <string>


namespace StructBX
{
    namespace Security
    {
        class User;
    }
}


class StructBX::Security::User
{
    public:
        User();
        User(int id, std::string username, int id_group);

        int get_id() const { return id_; }
        std::string get_username() const { return username_; }
        int get_id_group() const { return id_group_; }

        void set_id(int id) { id_ = id; }
        void set_username(std::string username) { username_ = username; }
        void set_id_group(int id_group) { id_group_ = id_group; }

    private:
        int id_;
        std::string username_;
        int id_group_;
};


#endif // STRUCTBX_SECURITY_USER
