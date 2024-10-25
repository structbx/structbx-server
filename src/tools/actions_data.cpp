
#include "tools/actions_data.h"
#include <functions/action.h>

StructBI::Tools::ActionsData::ActionsData(StructBI::Tools::FunctionData& function_data) : 
    spaces_(function_data)
    ,forms_(function_data)
    ,forms_data_(function_data)
    ,forms_columns_(function_data)
{
    
}

void StructBI::Tools::ActionsData::Spaces::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);

}

void StructBI::Tools::ActionsData::Spaces::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    if(get_space_id() == "")
    {
        // Request space ID in DB (Not found in cookie)
        action_->set_sql_code(
            "SELECT s.* " \
            "FROM spaces s " \
            "JOIN spaces_users su ON su.id_space = s.id " \
            "WHERE su.id_naf_user = ?"
        );
        action_->AddParameter_("id_naf_user", get_id_user(), false);
    }
    else
    {
        // Request space ID in Cookie
        action_->set_sql_code(
            "SELECT s.* " \
            "FROM spaces s " \
            "JOIN spaces_users su ON su.id_space = s.id " \
            "WHERE su.id_naf_user = ? AND s.id = ?"
        );
        action_->AddParameter_("id_naf_user", get_id_user(), false);
        action_->AddParameter_("id_space", get_space_id(), false);
    }
}

void StructBI::Tools::ActionsData::Spaces::AddA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT s.id FROM spaces s WHERE s.identifier = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un espacio con este identificador ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void StructBI::Tools::ActionsData::Spaces::AddA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO spaces (identifier, name, description) VALUES (?, ?, ?)"
    );
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador del espacio no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() <= 3)
        {
            param->set_error("El identificador debe tener más de 3 caracteres");
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
        if(param->ToString_() == "")
        {
            param->set_error("El nombre del espacio no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() <= 3)
        {
            param->set_error("El nombre del espacio debe tener más de 3 caracteres");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);
}

void StructBI::Tools::ActionsData::Spaces::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO spaces_users (id_naf_user, id_space) " \
        "SELECT ?, s.id FROM spaces s WHERE identifier = ?"
    );
    action_->AddParameter_("id_space", get_space_id(), false);
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador del espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void StructBI::Tools::ActionsData::Spaces::ChangeA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.id, s.name, s.state, s.logo, s.description, s.created_at " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id_space", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-id_space", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de cambio de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void StructBI::Tools::ActionsData::Spaces::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE spaces o " \
        "JOIN spaces_users ou ON ou.id_space = o.id " \
        "SET o.name = ?, o.description = ? " \
        "WHERE ou.id_naf_user = ? AND o.id = ?"
    );

    // Parameters and conditions
    action_->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);

    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id_space", get_space_id(), false);

}

void StructBI::Tools::ActionsData::Forms::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action->set_sql_code(
        "SELECT " \
            "f.* " \
        "FROM forms f " \
        "WHERE " \
            "id_space = ? "
    );
    action->AddParameter_("id_space", get_space_id(), false);
}

void StructBI::Tools::ActionsData::Forms::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms WHERE id = ? AND id_space = ?");

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

void StructBI::Tools::ActionsData::Forms::ReadSpecificA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms WHERE identifier = ? AND id_space = ?");

    action_->AddParameter_("identifier", "", true)
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

void StructBI::Tools::ActionsData::Forms::AddA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador para este espacio ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void StructBI::Tools::ActionsData::Forms::AddA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("INSERT INTO forms (identifier, name, state, privacity, description, id_space) VALUES (?, ?, ?, ?, ?, ?)");

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
    action_->AddParameter_("state", "", true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("privacity", "", true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
}

void StructBI::Tools::ActionsData::Forms::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, length, required, id_column_type, id_form) " \
        "SELECT " \
            "? " \
            ",? " \
            ",? " \
            ",? " \
            ",(SELECT id FROM forms_columns_types WHERE identifier = 'int-number') " \
            ",(SELECT id FROM forms WHERE identifier = ? and id_space = ?) "
    );

    action_->AddParameter_("identifier", "id", false);
    action_->AddParameter_("name", "ID", false);
    action_->AddParameter_("length", "11", false);
    action_->AddParameter_("required", 1, false);
    action_->AddParameter_("identifier", "", true);
    action_->AddParameter_("space_id", get_space_id(), false);
}

void StructBI::Tools::ActionsData::Forms::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier, id_space FROM forms WHERE id = ?");
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void StructBI::Tools::ActionsData::Forms::ModifyA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id != ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador en este espacio ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

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

void StructBI::Tools::ActionsData::Forms::ModifyA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE forms " \
        "SET identifier = ?, name = ?, state = ?, privacity = ?, description = ? " \
        "WHERE id = ? AND id_space = ?"
    );

    // Parameters and conditions
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
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
    action_->AddParameter_("state", "", true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("privacity", "", true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);

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
    action_->AddParameter_("id_space", get_space_id(), false);

}

void StructBI::Tools::ActionsData::Forms::DeleteA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier FROM forms WHERE id = ?");
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void StructBI::Tools::ActionsData::Forms::DeleteA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("DELETE FROM forms WHERE id = ? AND id_space = ?");
    action_->AddParameter_("id", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
}

void StructBI::Tools::ActionsData::FormsData::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ? " \
        "ORDER BY fc.position ASC"
    );
    action_->set_final(false);
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
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

void StructBI::Tools::ActionsData::FormsData::ReadColumnsA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ? AND id_space = ?");
    action_->set_final(false);
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void StructBI::Tools::ActionsData::FormsData::ReadColumnsA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fc.identifier != 'id' " \
        "ORDER BY fc.position ASC"
    );
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void StructBI::Tools::ActionsData::FormsData::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
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

void StructBI::Tools::ActionsData::FormsData::ReadSpecificA02::Setup_(Functions::Action::Ptr action)
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

void StructBI::Tools::ActionsData::FormsData::AddA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ? AND id_space = ?");
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
    action_->AddParameter_("form-identifier", get_space_id(), false);

}

void StructBI::Tools::ActionsData::FormsData::AddA02::Setup_(Functions::Action::Ptr action)
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

void StructBI::Tools::ActionsData::FormsData::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;


}

void StructBI::Tools::ActionsData::FormsData::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ? AND id_space = ?");
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

void StructBI::Tools::ActionsData::FormsData::ModifyA02::Setup_(Functions::Action::Ptr action)
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

void StructBI::Tools::ActionsData::FormsData::ModifyA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

}

void StructBI::Tools::ActionsData::FormsData::DeleteA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ? AND id_space = ?");
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

void StructBI::Tools::ActionsData::FormsData::DeleteA02::Setup_(Functions::Action::Ptr action)
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

void StructBI::Tools::ActionsData::FormsColumns::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type, fct.name AS column_type_name " \
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

void StructBI::Tools::ActionsData::FormsColumns::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type, fct.name AS column_type_name " \
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

void StructBI::Tools::ActionsData::FormsColumns::ReadTypesA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms_columns_types ");
}

void StructBI::Tools::ActionsData::FormsColumns::AddA01::Setup_(Functions::Action::Ptr action)
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

void StructBI::Tools::ActionsData::FormsColumns::AddA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.identifier = ? AND f.identifier = ?"
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
}

void StructBI::Tools::ActionsData::FormsColumns::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;
    
    action_->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, position, length, required, default_value, description, id_column_type, id_form) " \
        "SELECT " \
            "?, ? " \
            ",MAX(fc.position) + 1 " \
            ",?, ?, ?, ?, ? " \
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

void StructBI::Tools::ActionsData::FormsColumns::ModifyA01::Setup_(Functions::Action::Ptr action)
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

void StructBI::Tools::ActionsData::FormsColumns::ModifyA02::Setup_(Functions::Action::Ptr action)
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

void StructBI::Tools::ActionsData::FormsColumns::ModifyA02_1::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.identifier " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.id = ?"
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
}

void StructBI::Tools::ActionsData::FormsColumns::ModifyA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;
    
    action_->set_sql_code(
        "UPDATE forms_columns SET identifier = ?, name = ?, length = ?, required = ?, default_value = ?, description = ?, id_column_type = ? " \
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

void StructBI::Tools::ActionsData::FormsColumns::DeleteA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fc.* FROM forms_columns fc " \
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

void StructBI::Tools::ActionsData::FormsColumns::DeleteA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

}

void StructBI::Tools::ActionsData::FormsColumns::DeleteA03::Setup_(Functions::Action::Ptr action)
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