
#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"

#include "functions/forms.h"
#include "functions/dashboards.h"
#include "functions/reports.h"
#include "functions/reports_graphs.h"
#include "functions/reports_parameters.h"
#include "functions/spaces.h"
#include "functions/spaces_users.h"

using namespace NAF;

class BackendServer : public Handlers::BackendHandler
{
    public:
        BackendServer() : Handlers::BackendHandler(){}
        virtual ~BackendServer() {}

        void AddFunctions_();

        void Process_() override;
};
