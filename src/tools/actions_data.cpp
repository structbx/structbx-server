
#include "tools/actions_data.h"
#include <functions/action.h>

using namespace StructBX::Tools;

ActionsData::ActionsData(FunctionData& function_data) :
    forms_data_(function_data)
    ,forms_columns_(function_data)
    ,forms_permissions_(function_data)
{
    
}

void ActionsData::FormsData::VerifyPermissionsRead::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.read = 1 AND fp.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](NAF::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
    action_->AddParameter_("id_user", get_id_user(), false);
}

void ActionsData::FormsData::VerifyPermissionsAdd::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.add = 1 AND fp.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](NAF::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
    action_->AddParameter_("id_user", get_id_user(), false);
}

void ActionsData::FormsData::VerifyPermissionsModify::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.modify = 1 AND fp.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](NAF::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
    action_->AddParameter_("id_user", get_id_user(), false);
}

void ActionsData::FormsData::VerifyPermissionsDelete::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.delete = 1 AND fp.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](NAF::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
    action_->AddParameter_("id_user", get_id_user(), false);
}

void ActionsData::FormsData::ReadA01_0::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id, fc.id AS column_id " \
        "FROM forms f " \
        "JOIN forms_columns fc ON fc.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fc.identifier = 'id'"
    );
    action_->set_final(false);
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ? " \
        "ORDER BY fc.position ASC"
    );
    action_->set_final(false);
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::ReadFileA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "WHERE f.identifier = ? AND f.id_space = ? "
    );
    action_->set_final(false);
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ? " \
        "ORDER BY fc.position ASC"
    );
    action_->set_final(false);
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::ReadSpecificA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::FormsData::AddA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);

}

void ActionsData::FormsData::AddA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action_->set_final(false);
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;


}

void ActionsData::FormsData::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id, fc.id AS column_id " \
        "FROM forms f " \
        "JOIN forms_columns fc ON fc.id_form = f.id " \
        "WHERE f.identifier = ? AND id_space = ? AND fc.identifier = 'id'"
    );
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::ModifyA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action_->set_final(false);
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::ModifyA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

}

void ActionsData::FormsData::DeleteA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT f.id, fc.id AS column_id " \
        "FROM forms f " \
        "JOIN forms_columns fc ON fc.id_form = f.id " \
        "WHERE f.identifier = ? AND id_space = ? AND fc.identifier = 'id'"
    );
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsData::DeleteA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de registro no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::FormsColumns::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "WHERE " \
            "id_space = ? AND f.identifier = ? " \
        "ORDER BY fc.position ASC"
    );

    action_->AddParameter_("id_space", get_space_id(), false);

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::FormsColumns::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "WHERE " \
            "fc.id = ? AND id_space = ? AND f.identifier = ? " \
        "ORDER BY fc.position ASC"
    );
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::FormsColumns::ReadTypesA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms_columns_types ");
}

void ActionsData::FormsColumns::AddA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsColumns::AddA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.identifier = ? AND f.identifier = ? AND id_space = ?"
    );

    action_->SetupCondition_("verify-column-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("La columna solicitada ya existe en el formulario actual");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de columna no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsColumns::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;
    
    action_->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, position, length, required, default_value, description, id_column_type, link_to, id_form) " \
        "SELECT " \
            "?, ? " \
            ",MAX(fc.position) + 1 " \
            ",?, ?, ?, ?, ?, ? " \
            ",f.id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        auto string_param = param->get_value()->ToString_();
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(string_param == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(string_param.size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().Check_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });

    action_->AddParameter_("length", "", true);
    action_->AddParameter_("required", "", true)
    ->SetupCondition_("condition-required", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "1" || param->get_value()->ToString_() == "0")
        {
            return true;
        }
        else
        {
            param->set_error("El valor de obligatorio debe ser boleano");
            return false;
        }
        return true;
    });
    action_->AddParameter_("default_value", "", true);
    action_->AddParameter_("description", "", true);
    action_->AddParameter_("id_column_type", "", true)
    ->SetupCondition_("condition-id_column_type", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El tipo de columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("link_to", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-link_to", Query::ConditionType::kWarning, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()));
        }
        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsColumns::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsColumns::ModifyA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.identifier, fc.id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.id != ? AND fc.identifier = ? AND f.identifier = ?"
    );

    action_->SetupCondition_("verify-column-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("La columna solicitada ya existe en el formulario actual");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de columna no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::FormsColumns::ModifyA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;
    
    action_->set_sql_code(
        "UPDATE forms_columns SET " \
            "identifier = ?, name = ?, length = ?, required = ? " \
            ",default_value = ?, description = ?, id_column_type = ?, link_to = ?, position = ? " \
        "WHERE id = ? AND id_form = (SELECT id FROM forms WHERE identifier = ? AND id_space = ? LIMIT 1)"
    );

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        auto string_param = param->get_value()->ToString_();
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(string_param == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(string_param.size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().Check_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action_->AddParameter_("length", "", true);
    action_->AddParameter_("required", "", true)
    ->SetupCondition_("condition-required", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "1" || param->get_value()->ToString_() == "0")
        {
            return true;
        }
        else
        {
            param->set_error("El valor de obligatorio debe ser boleano");
            return false;
        }
        return true;
    });
    action_->AddParameter_("default_value", "", true);
    action_->AddParameter_("description", "", true);
    action_->AddParameter_("id_column_type", "", true)
    ->SetupCondition_("condition-id_column_type", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El tipo de columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("link_to", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-id_column_type", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()));
        }
        return true;
    });
    action_->AddParameter_("position", "", true)
    ->SetupCondition_("condition-position", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La posici&oacute;n no puede estar vacía");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de la columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsColumns::DeleteA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.id AS column_id, f.id AS form_id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.id = ? AND f.identifier = ? AND f.id_space = ?"
    );
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("La columna solicitada no existe en el formulario actual");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::FormsColumns::DeleteA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

}

void ActionsData::FormsColumns::DeleteA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("DELETE FROM forms_columns WHERE id = ?");

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::FormsPermissions::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fp.*, nu.username AS username, f.name AS form_name " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "JOIN _naf_users nu ON nu.id = fp.id_naf_user "
        "WHERE f.identifier = ? AND f.id_space = ?"
    );

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}
