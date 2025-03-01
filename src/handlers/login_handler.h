
#ifndef STRUCTBX_HANDLERS_LOGINHANDLER
#define STRUCTBX_HANDLERS_LOGINHANDLER


#include "handlers/root_handler.h"


namespace StructBX
{
    namespace Handlers
    {
        struct AuthRoutes;
        class LoginHandler;
    }
}


struct StructBX::Handlers::AuthRoutes
{
    static Tools::Route login_route;
    static Tools::Route logout_route;
};

class StructBX::Handlers::LoginHandler :
    public RootHandler
{
    public:
        using RootHandler::RootHandler;
        virtual ~LoginHandler();

    protected:
        virtual void Process_() override;
        virtual void Login_();
        void StartSession_();
        void EndSession_();
        std::string SessionExists_();

    private:
};

#endif // STRUCTBX_HANDLERS_LOGINHANDLER
