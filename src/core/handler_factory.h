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

#ifndef NAF_COREANDLER_FACTORY
#define NAF_COREANDLER_FACTORY


#include <map>
#include <exception>
#include <vector>
#include <memory>
#include <stdexcept>

#include "Poco/Exception.h"
#include "Poco/Format.h"
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/StreamCopier.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>

#include "tools/handler_connection.h"
#include "tools/sessions_manager.h"
#include "query/database_manager.h"
#include "tools/route.h"
#include "http/request.h"
#include "handlers/root_handler.h"
#include "handlers/null_handler.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "handlers/websocket_handler.h"
#include "tools/output_logger.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Data::Keywords;

namespace NAF
{
    namespace Core
    {
        struct HTTPRequestInfo;
        class HandlerFactory;
    }
}


struct NAF::Core::HTTPRequestInfo
{
    HTTPRequestInfo(std::string uri, std::string method) : uri(uri), method(method){}

    std::string uri;
    std::string method;
};

class NAF::Core::HandlerFactory :
    public HTTPRequestHandlerFactory
{
    public:
        using FunctionHandler = std::function<NAF::Handlers::RootHandler*()>;
        using FunctionHandlerCreator = std::function<NAF::Handlers::RootHandler*(Core::HTTPRequestInfo& info)>;
        using Connections = std::map<std::string, Tools::HandlerConnection>;

        HandlerFactory();
        virtual ~HandlerFactory();
        
        Connections& get_connections()
        {
            auto& var = connections_;
            return var;
        }
        FunctionHandlerCreator& get_handler_creator_()
        {
            auto& var = handler_creator_;
            return var;
        }
        void set_handler_creator(FunctionHandlerCreator handler_creator){ handler_creator_ = handler_creator; }

        virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

    protected:
        void ErrorResponse_(const HTTPServerRequest& request, std::string error);

    private:
        FunctionHandlerCreator handler_creator_;
        Connections connections_;
};

#endif // NAF_COREANDLER_FACTORY
