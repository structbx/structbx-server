
#ifndef STRUCTBX_CONTROLLERS_SPACES_MAIN_H
#define STRUCTBX_CONTROLLERS_SPACES_MAIN_H

#include "Poco/DirectoryIterator.h"

#include "tools/function_data.h"
#include "tools/base64_tool.h"

#include "tools/base_action.h"
#include "controllers/spaces/users.h"

namespace StructBX
{
    namespace Controllers
    {
        namespace Spaces
        {
            class Main;
        }
    }
}

using namespace StructBX;


class StructBX::Controllers::Spaces::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);
        
    protected:
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };
        struct ReadSpecific : public Tools::FunctionData
        {
            ReadSpecific(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };
        struct Change : public Tools::FunctionData
        {
            Change(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
            void A2(StructBX::Functions::Action::Ptr action);
            void A3(StructBX::Functions::Action::Ptr action);
        };
        struct Modify : public Tools::FunctionData
        {
            Modify(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
            void A2(StructBX::Functions::Action::Ptr action);
            void A3(StructBX::Functions::Action::Ptr action);
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
            void A2(StructBX::Functions::Action::Ptr action);
            void A3(StructBX::Functions::Action::Ptr action);
        };

    private:
        Users function_users_;
        Read struct_read_;
        ReadSpecific struct_read_specific_;
        Change struct_change_;
        Add struct_add_;
        Modify struct_modify_;
        Delete struct_delete_;
};

#endif //STRUCTBX_CONTROLLERS_SPACES_MAIN_H