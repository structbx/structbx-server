
#ifndef STRUCTBX_TOOLS_ACTIONSDATA
#define STRUCTBX_TOOLS_ACTIONSDATA

#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Tools
    {
        class ActionsData;
    }
}

using namespace StructBX;

class StructBX::Tools::ActionsData
{
    public:
        ActionsData(Tools::FunctionData& function_data);

        /*
        struct ElementName
        {
            ElementName(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
            {
                
            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;
        } element_name_;
        */

        struct Spaces
        {
            Spaces(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
                ,read_specific_a01_(function_data)
                ,add_a01_(function_data)
                ,add_a02_(function_data)
                ,add_a03_(function_data)
                ,change_a01_(function_data)
                ,modify_a01_(function_data)
                ,modify_a02_(function_data)
                ,modify_a03_(function_data)
                ,delete_a01_(function_data)
                ,delete_a02_(function_data)
                ,delete_a03_(function_data)
            {
                
            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;

            class ReadSpecificA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_specific_a01_;

            class AddA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a01_;

            class AddA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a02_;

            class AddA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a03_;

            class ChangeA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } change_a01_;

            class ModifyA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;
                    
            } modify_a01_;

            class ModifyA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;
                    
            } modify_a02_;

            class ModifyA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;
                    
            } modify_a03_;

            class DeleteA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;
                    
            } delete_a01_;

            class DeleteA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;
                    
            } delete_a02_;

            class DeleteA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;
                    
            } delete_a03_;

        } spaces_;

        struct Forms
        {
            Forms(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
                ,read_specific_a01_(function_data)
                ,read_specific_a02_(function_data)
                ,add_a01_(function_data)
                ,add_a02_(function_data)
                ,add_a03_(function_data)
                ,add_a03_1_(function_data)
                ,modify_a01_(function_data)
                ,modify_a02_(function_data)
                ,modify_a03_(function_data)
                ,delete_a01_(function_data)
                ,delete_a02_(function_data)
            {
                
            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(NAF::Functions::Action::Ptr action) override;

            } read_a01_;

            class ReadSpecificA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_specific_a01_;

            class ReadSpecificA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_specific_a02_;

            class AddA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a01_;

            class AddA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a02_;

            class AddA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a03_;

            class AddA03_1 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a03_1_;

            class ModifyA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a01_;

            class ModifyA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a02_;

            class ModifyA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a03_;

            class DeleteA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a01_;

            class DeleteA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a02_;

        } forms_;

        struct FormsData
        {
            FormsData(Tools::FunctionData& function_data) : 
                verify_permissions_read_(function_data)
                ,verify_permissions_add_(function_data)
                ,verify_permissions_modify_(function_data)
                ,verify_permissions_delete_(function_data)
                ,read_a01_0_(function_data)
                ,read_a01_(function_data)
                ,read_file_a01_(function_data)
                ,read_specific_a01_(function_data)
                ,read_specific_a02_(function_data)
                ,add_01_(function_data)
                ,add_02_(function_data)
                ,add_03_(function_data)
                ,modify_01_(function_data)
                ,modify_02_(function_data)
                ,modify_03_(function_data)
                ,delete_a01_(function_data)
                ,delete_a02_(function_data)
            {
                
            }

            class VerifyPermissionsRead : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } verify_permissions_read_;

            class VerifyPermissionsAdd : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } verify_permissions_add_;

            class VerifyPermissionsModify : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } verify_permissions_modify_;

            class VerifyPermissionsDelete : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } verify_permissions_delete_;

            class ReadA01_0 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_0_;

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;

            class ReadFileA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_file_a01_;

            class ReadSpecificA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_specific_a01_;

            class ReadSpecificA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_specific_a02_;

            class AddA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_01_;

            class AddA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_02_;

            class AddA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_03_;

            class ModifyA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_01_;

            class ModifyA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_02_;

            class ModifyA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_03_;

            class DeleteA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a01_;

            class DeleteA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a02_;
        } forms_data_;

        struct FormsColumns
        {
            FormsColumns(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
                ,read_specific_a01_(function_data)
                ,read_types_a01_(function_data)
                ,add_a01_(function_data)
                ,add_a02_(function_data)
                ,add_a03_(function_data)
                ,modify_a01_(function_data)
                ,modify_a02_(function_data)
                ,modify_a03_(function_data)
                ,delete_a01_(function_data)
                ,delete_a02_(function_data)
                ,delete_a03_(function_data)
            {

            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;

            class ReadSpecificA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_specific_a01_;

            class ReadTypesA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_types_a01_;

            class AddA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a01_;

            class AddA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a02_;

            class AddA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a03_;

            class ModifyA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a01_;

            class ModifyA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a02_;

            class ModifyA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a03_;

            class DeleteA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a01_;

            class DeleteA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a02_;

            class DeleteA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a03_;

        } forms_columns_;

        struct FormsPermissions
        {
            FormsPermissions(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
            {
                
            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;
        } forms_permissions_;
};

#endif //STRUCTBX_TOOLS_ACTIONSDATA