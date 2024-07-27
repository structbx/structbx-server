

#include <list>

#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "functions/function.h"
#include "tools/dvalue.h"

using namespace Atom;

class Reports
{
    public:
        Reports();

        std::list<Functions::Function::Ptr>& get_functions()
        {
            auto& var = functions_;
            return var;
        }

    protected:
        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        std::list<Functions::Function::Ptr> functions_;
};
