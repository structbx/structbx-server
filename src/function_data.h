

#ifndef STRUCTBI_FUNCTIONDATA
#define STRUCTBI_FUNCTIONDATA

#include <list>

#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "functions/function.h"
#include "tools/dvalue.h"

#include "tools/id_checker.h"

using namespace NAF;

class FunctionData
{
    public:
        using FunctionsList = std::shared_ptr<std::list<NAF::Functions::Function::Ptr>>;

        FunctionData()
        {
            functions_ = std::make_shared<std::list<NAF::Functions::Function::Ptr>>();
        }

        int get_id_user(){ return id_user_; }
        FunctionsList& get_functions()
        {
            auto& var = functions_;
            return var;
        }

        void set_id_user(int id_user){ id_user_ = id_user; }
        void set_functions(FunctionsList functions){ functions_ = functions; }

    private:
        int id_user_;
        FunctionsList functions_;
};

#endif //STRUCTBI_FUNCTIONDATA