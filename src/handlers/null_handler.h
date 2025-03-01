
#ifndef STRUCTBX_HANDLERS_NULLHANDLER
#define STRUCTBX_HANDLERS_NULLHANDLER


#include "handlers/root_handler.h"


namespace StructBX
{
    namespace Handlers
    {
        class NullHandler;
    }
}


class StructBX::Handlers::NullHandler :
    public RootHandler
{
    public:
        using RootHandler::RootHandler;
        virtual ~NullHandler();

    protected:
        virtual void Process_() override;

    private:
        void NullResponse_();
};

#endif // STRUCTBX_HANDLERS_NULLHANDLER
