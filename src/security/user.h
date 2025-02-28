/*
* <one line to give the program's name and a brief idea of what it does.>
* Copyright (C) 2021  <copyright holder> <email>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef USER
#define USER


#include <string>


namespace NAF
{
    namespace Security
    {
        class User;
    }
}


class NAF::Security::User
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


#endif // USER
