
#include "core/nebula_atom.h"
#include "handlers/root_handler.h"

using namespace NAF;

class Webserver : public Handlers::RootHandler
{
    public:
        Webserver();

        void Process_();
        void DownloadProcess_();

    private:
        Files::FileManager file_manager_;
};
