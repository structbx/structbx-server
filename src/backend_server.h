
#ifndef STRUCTBI_BACKENDSERVER
#define STRUCTBI_BACKENDSERVER

#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"

#include "function_data.h"
#include "functions/spaces.h"
#include "functions/forms.h"

using namespace NAF;

class BackendServer : public Handlers::BackendHandler
{
    public:
        BackendServer();
        virtual ~BackendServer() {}

        void AddFunctions_();

        void Process_() override;

    protected:
        void SetupFunctionData_();

    private:
        FunctionData function_data_;
};

#endif //STRUCTBI_BACKENDSERVER