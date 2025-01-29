
#ifndef STRUCTBX_FUNCTIONS_FORMS_MAIN_H
#define STRUCTBX_FUNCTIONS_FORMS_MAIN_H

#include "tools/base_action.h"
#include "tools/function_data.h"

#include "functions/forms/data.h"
#include "functions/forms/columns.h"
#include "functions/forms/permissions.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Forms
        {
            struct MainData;
            class Main;
        }
    }
}

using namespace StructBX;
using namespace NAF;


class StructBX::Functions::Forms::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);

    protected:
        struct ActionsData : public Tools::FunctionData
        {
            ActionsData(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
            {
            }

            void ReadA01(NAF::Functions::Action::Ptr action);
            void ReadSpecificA01(NAF::Functions::Action::Ptr action);
            void ReadSpecificA02(NAF::Functions::Action::Ptr action);
            void AddA01(NAF::Functions::Action::Ptr action);
            void AddA02(NAF::Functions::Action::Ptr action);
            void AddA03(NAF::Functions::Action::Ptr action);
            void AddA03_1(NAF::Functions::Action::Ptr action);
            void ModifyA01(NAF::Functions::Action::Ptr action);
            void ModifyA02(NAF::Functions::Action::Ptr action);
            void ModifyA03(NAF::Functions::Action::Ptr action);
            void DeleteA01(NAF::Functions::Action::Ptr action);
            void DeleteA02(NAF::Functions::Action::Ptr action);
        };

        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        ActionsData actions_;
        Data data_;
        Columns columns_;
        Permissions permissions_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_MAIN_H