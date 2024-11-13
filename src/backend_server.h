
#ifndef STRUCTBI_BACKENDSERVER
#define STRUCTBI_BACKENDSERVER

#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"

#include "tools/function_data.h"
#include "functions/organizations.h"
#include "functions/spaces/spaces.h"
#include "functions/spaces/spaces_users.h"
#include "functions/forms/forms.h"
#include "functions/forms/forms_data.h"
#include "functions/forms/forms_columns.h"

namespace StructBI
{
    class BackendServer;
}

using namespace StructBI;
using namespace NAF;

class StructBI::BackendServer : public Handlers::BackendHandler
{
    public:
        BackendServer();
        virtual ~BackendServer() {}

        void AddFunctions_();

        void Process_() override;

    protected:
        void SetupFunctionData_();

    private:
        Tools::FunctionData function_data_;
        HTTP::Cookie space_id_cookie_;
        bool add_space_id_cookie_;
};

#endif //STRUCTBI_BACKENDSERVER