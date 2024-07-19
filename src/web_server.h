
#include "core/nebula_atom.h"
#include "handlers/frontend_handler.h"

using namespace Atom;

class Webserver : public Handlers::FrontendHandler
{
    public:
        Webserver();
        virtual ~Webserver(){}

        void Process_();
};
