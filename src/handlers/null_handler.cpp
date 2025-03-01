
#include "handlers/null_handler.h"

using namespace StructBX::Handlers;

NullHandler::~NullHandler()
{

}

void NullHandler::Process_()
{
    HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Sorry, content not found from " + get_http_server_request().value()->getMethod() + " request.");
}
