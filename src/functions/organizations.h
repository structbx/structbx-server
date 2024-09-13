

#include <string>
#include <list>

#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "functions/function.h"
#include "tools/dvalue.h"
#include <files/file_manager.h>
#include <functions/action.h>
#include <tools/output_logger.h>
#include <tools/settings_manager.h>

using namespace NAF;

class Organizations
{
    public:
        Organizations(int id_user_);

        std::list<Functions::Function::Ptr>& get_functions()
        {
            auto& var = functions_;
            return var;
        }

    protected:
        void Read_();
        void ReadSpecific_();
        void ReadLogo_();
        void Add_();
        void Modify_();
        void ModifyLogo_();
        void Delete_();

    private:
        int id_user_;
        std::list<Functions::Function::Ptr> functions_;
};
