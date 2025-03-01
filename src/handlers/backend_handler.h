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

#ifndef STRUCTBX_HANDLERS_BACKENDHANDLER
#define STRUCTBX_HANDLERS_BACKENDHANDLER


#include "handlers/root_handler.h"
#include "query/results.h"
#include "tools/function_data.h"
#include "functions/general/main.h"
#include "functions/spaces/main.h"
#include "functions/forms/main.h"


namespace StructBX
{
    namespace Handlers
    {
        class BackendHandler;
    }
}


class StructBX::Handlers::BackendHandler : public RootHandler
{
    public:
        using VectorShrResults = std::vector<Query::Results::Ptr>;

        BackendHandler();

        void AddFunctions_();

    protected:
        virtual void Process_() override;
        void ProcessActions_();
        void SetupFunctionData_();
        bool VerifyActiveUser_();

    private:
        Tools::FunctionData function_data_;
        HTTP::Cookie space_id_cookie_;
        bool add_space_id_cookie_;
};

#endif // STRUCTBX_HANDLERS_BACKENDHANDLER
