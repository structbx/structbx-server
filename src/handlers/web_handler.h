

#ifndef STRUCTBX_HANDLERS_WEBHANDLER
#define STRUCTBX_HANDLERS_WEBHANDLER

#include "handlers/root_handler.h"

namespace StructBX
{
    namespace Handlers
    {
        class WebHandler;
    }
}

using namespace StructBX;

class StructBX::Handlers::WebHandler : public Handlers::RootHandler
{
    public:
        WebHandler();

        void Process_();
        void DownloadProcess_();

    private:
        Files::FileManager file_manager_;
};

#endif //STRUCTBX_HANDLERS_WEBHANDLER