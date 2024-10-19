

#ifndef STRUCTBI_WEBSERVER
#define STRUCTBI_WEBSERVER

#include "core/nebula_atom.h"
#include "handlers/root_handler.h"

using namespace NAF;

namespace StructBI
{
    class Webserver;
}

using namespace StructBI;
using namespace NAF;

class StructBI::Webserver : public Handlers::RootHandler
{
    public:
        Webserver();

        void Process_();
        void DownloadProcess_();

    private:
        Files::FileManager file_manager_;
};

#endif //STRUCTBI_WEBSERVER