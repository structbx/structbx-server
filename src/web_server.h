

#ifndef STRUCTBX_WEBSERVER
#define STRUCTBX_WEBSERVER

#include "core/nebula_atom.h"
#include "handlers/root_handler.h"

namespace StructBX
{
    class Webserver;
}

using namespace StructBX;

class StructBX::Webserver : public Handlers::RootHandler
{
    public:
        Webserver();

        void Process_();
        void DownloadProcess_();

    private:
        Files::FileManager file_manager_;
};

#endif //STRUCTBX_WEBSERVER