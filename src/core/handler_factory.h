
#ifndef STRUCTBX_CORE_HANDLERFACTORY
#define STRUCTBX_CORE_HANDLERFACTORY


#include <map>
#include <exception>
#include <vector>
#include <memory>
#include <stdexcept>

#include <Poco/Exception.h>
#include <Poco/Format.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/StreamCopier.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>

#include "web_server.h"
#include "sessions/sessions_manager.h"
#include "query/database_manager.h"
#include "tools/route.h"
#include "http/request.h"
#include "handlers/root_handler.h"
#include "handlers/null_handler.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "tools/output_logger.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Data::Keywords;

namespace StructBX
{
    namespace Core
    {
        struct HTTPRequestInfo;
        class HandlerFactory;
    }
}


class StructBX::Core::HandlerFactory :
    public HTTPRequestHandlerFactory
{
    public:
        HandlerFactory();
        
        virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

    protected:
        void ErrorResponse_(const HTTPServerRequest& request, std::string error);
};

#endif // STRUCTBX_CORE_HANDLERFACTORY
