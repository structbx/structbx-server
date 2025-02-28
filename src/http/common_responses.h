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

#ifndef STRUCTBX_HTTP_COMMONRESPONSES
#define STRUCTBX_HTTP_COMMONRESPONSES


#include <string>
#include <map>

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>

#include "nebulaatomConfig.h"
#include "files/file_manager.h"
#include "http/request.h"


namespace NAF
{
    namespace HTTP
    {
        enum class ResponseType;
        enum class Status;
        class CommonResponses;
    }
}

using namespace Poco;
using namespace Poco::Net;


enum class NAF::HTTP::ResponseType
{
    kError
    ,kWarning
    ,kInformation
    ,kSuccess
};

enum class NAF::HTTP::Status
{
    kHTTP_OK = 200
    ,kHTTP_BAD_REQUEST = 400
    ,kHTTP_UNAUTHORIZED = 401
    ,kHTTP_FORBIDDEN = 403
    ,kHTTP_NOT_FOUND = 404
    ,kHTTP_INTERNAL_SERVER_ERROR = 500
    ,kHTTP_BAD_GATEWAY = 502
    ,kHTTP_SERVICE_UNAVAILABLE = 503
};

class NAF::HTTP::CommonResponses : public HTTP::Request
{
    public:
        struct Attributes
        {
            HTTPResponse::HTTPStatus http_status;
            int status_int;
            ResponseType response_type;
            std::string message;
        };

        CommonResponses();
        ~CommonResponses();

        std::map<HTTP::Status, Attributes>& get_responses_()
        {
            auto& var = responses_;
            return var;
        }

        void CompoundResponse_(HTTP::Status status, JSON::Object::Ptr result_json);
        void CompoundFillResponse_(HTTP::Status status, JSON::Object::Ptr result_json, std::string message);
        void JSONResponse_(HTTP::Status status, std::string message);
        void HTMLResponse_(HTTP::Status status, std::string message);
        void CustomHTMLResponse_(HTTP::Status status, std::string html_message);
        void FileResponse_(HTTP::Status status, std::string address);
        void FileResponse_(HTTP::Status status, std::string address, Files::FileManager& file_manager);

    protected:
        void FillResponses_();
        void FillStatusMessage_(JSON::Object::Ptr json_object, HTTP::Status status, std::string message);

    private:
        std::map<HTTP::Status, Attributes> responses_;
};

#endif // STRUCTBX_HTTP_COMMONRESPONSES
