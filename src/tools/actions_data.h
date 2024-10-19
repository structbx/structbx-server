
#ifndef STRUCTBI_TOOLS_ACTIONSDATA
#define STRUCTBI_TOOLS_ACTIONSDATA

#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBI
{
    namespace Tools
    {
        class ActionsData;
    }
}

using namespace StructBI;

class StructBI::Tools::ActionsData
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
                ,change_a01_(function_data)
                ,modify_a01_(function_data)
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

            class Delete01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a01_;

            class Delete02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a02_;

        } forms_;

        struct FormsData
        {
            FormsData(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
                ,read_columns_a01_(function_data)
                ,read_columns_a02_(function_data)
                ,read_specific_a01_(function_data)
                ,read_specific_a02_(function_data)
                ,add_01_(function_data)
                ,add_02_(function_data)
                ,add_03_(function_data)
            {
                
            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;

            class ReadColumnsA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_columns_a01_;

            class ReadColumnsA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_columns_a02_;

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
        } forms_data_;
};

#endif //STRUCTBI_TOOLS_ACTIONSDATA