
#ifndef STRUCTBX_BACKENDSERVER
#define STRUCTBX_BACKENDSERVER

#include "core/core.h"
#include "handlers/backend_handler.h"

#include "tools/function_data.h"
#include "functions/general/main.h"
#include "functions/spaces/main.h"
#include "functions/forms/main.h"

namespace StructBX
{
    class BackendServer;
}

using namespace StructBX;

class StructBX::BackendServer : public Handlers::BackendHandler
{
    public:
        BackendServer();
        virtual ~BackendServer() {}

        void AddFunctions_();

        void Process_() override;

    protected:
        void SetupFunctionData_();
        bool VerifyActiveUser_();

    private:
        Tools::FunctionData function_data_;
        HTTP::Cookie space_id_cookie_;
        bool add_space_id_cookie_;
};

#endif //STRUCTBX_BACKENDSERVER