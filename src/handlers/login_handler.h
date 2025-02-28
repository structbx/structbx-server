/*
* Nebula Atom

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

#ifndef STRUCTBX_HANDLERS_LOGINHANDLER
#define STRUCTBX_HANDLERS_LOGINHANDLER


#include "handlers/root_handler.h"


namespace NAF
{
    namespace Handlers
    {
        class LoginHandler;
    }
}


class NAF::Handlers::LoginHandler :
    public RootHandler
{
    public:
        using RootHandler::RootHandler;
        virtual ~LoginHandler();

    protected:
        virtual void Process_() override;
        virtual void Login_();
        void StartSession_();
        void EndSession_();
        std::string SessionExists_();

    private:
};

#endif // STRUCTBX_HANDLERS_LOGINHANDLER
