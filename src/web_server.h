

#ifndef STRUCTBX_WEBSERVER
#define STRUCTBX_WEBSERVER

#include "core/nebula_atom.h"
#include "handlers/root_handler.h"

using namespace NAF;

namespace StructBX
{
    class Webserver;
}

using namespace StructBX;
using namespace NAF;

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