
#include "tools/actions_data.h"

using namespace StructBI::Tools;

ActionsData::ActionsData(FunctionData& function_data) : 
    organizations_users_(function_data)
    ,spaces_(function_data)
    ,forms_(function_data)
    ,forms_data_(function_data)
    ,forms_columns_(function_data)
{
    
}

void ActionsData::OrganizationsUsers::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM _naf_users WHERE username = ? AND id != ?");
    action_->SetupCondition_("verify-username-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este nombre de usuario ya est&aacute; registrado");
            return false;
        }

        return true;
    });

    action_->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id", get_id_user(), false);
}

void ActionsData::OrganizationsUsers::ModifyA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE _naf_users "
        "SET username = ? "
        "WHERE id = ?"
    );

    action_->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void ActionsData::OrganizationsUsers::ModifyPasswordA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM _naf_users WHERE password = ? AND id = ?");
    action_->SetupCondition_("verify-username-password", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("La contrase&ntilde;a actual es incorrecta");
            return false;
        }

        return true;
    });

    action_->AddParameter_("current_password", "", true)
    ->SetupCondition_("condition-current_password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La contrase&ntilde;a actual no puede estar vacía");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));

        return true;
    });
    action_->AddParameter_("id", get_id_user(), false);
}

void ActionsData::OrganizationsUsers::ModifyPasswordA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE _naf_users "
        "SET password = ? "
        "WHERE id = ?"
    );

    action_->AddParameter_("new_password", "", true)
    ->SetupCondition_("condition-password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("La contraseña no puede estar vacía");
            return false;
        }
        if(param->ToString_().size() < 8)
        {
            param->set_error("La contraseña no puede ser menor a 8 dígitos");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));
        return true;
    });
    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void ActionsData::OrganizationsUsers::AddA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT id FROM _naf_users WHERE username = ?");
    action_->SetupCondition_("verify-username-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este nombre de usuario ya est&aacute; registrado");
            return false;
        }

        return true;
    });

    action_->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::OrganizationsUsers::AddA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO _naf_users (username, password, status, id_group) "
        "VALUES (?, ?, ?, ?) "
    );

    action_->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("password", "", true)
    ->SetupCondition_("condition-password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("La contraseña no puede estar vacía");
            return false;
        }
        if(param->ToString_().size() < 8)
        {
            param->set_error("La contraseña no puede ser menor a 8 dígitos");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));
        return true;
    });
    action_->AddParameter_("status", "activo", false);
    action_->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::OrganizationsUsers::ModifyUserA01_0::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT id FROM _naf_users WHERE username = ? AND id != ?");
    action_->SetupCondition_("verify-username-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este nombre de usuario ya est&aacute; registrado");
            return false;
        }

        return true;
    });

    action_->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::OrganizationsUsers::ModifyUserA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE _naf_users n "
        "JOIN organizations_users ou ON ou.id_naf_user = n.id "
        "SET username = ?, password = ?, status = ?, id_group = ? "
        "WHERE n.id = ? "
            "AND ou.id_organization = (SELECT id_organization FROM organizations_users WHERE id_naf_user = ? LIMIT 1)"
    );

    action_->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("password", "", true)
    ->SetupCondition_("condition-password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("La contraseña no puede estar vacía");
            return false;
        }
        if(param->ToString_().size() < 8)
        {
            param->set_error("La contraseña no puede ser menor a 8 dígitos");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));
        return true;
    });
    action_->AddParameter_("status", "", true)
    ->SetupCondition_("condition-status", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id del usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void ActionsData::OrganizationsUsers::ModifyUserA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE _naf_users n "
        "JOIN organizations_users ou ON ou.id_naf_user = n.id "
        "SET username = ?, status = ?, id_group = ? "
        "WHERE n.id = ? "
            "AND ou.id_organization = (SELECT id_organization FROM organizations_users WHERE id_naf_user = ? LIMIT 1)"
    );

    action_->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("status", "", true)
    ->SetupCondition_("condition-status", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id del usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void ActionsData::Spaces::ReadA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Spaces::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::Spaces::AddA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Spaces::AddA02::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Spaces::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO spaces_users (id_naf_user, id_space) " \
        "SELECT ?, s.id FROM spaces s WHERE identifier = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("identifier", "", true);
}

void ActionsData::Spaces::ChangeA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Spaces::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.id " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-user-in-space", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El usuario actual no pertenece al espacio que intenta modificar");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void ActionsData::Spaces::ModifyA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM spaces WHERE identifier = ? AND id != ?");
    action_->SetupCondition_("verify-space-identifier", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un espacio con este identificador ya existe");
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

void ActionsData::Spaces::ModifyA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "SET s.identifier = ?, s.name = ?, s.description = ? " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
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
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::Spaces::DeleteA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.id " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-user-in-space", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El usuario actual no pertenece al espacio que intenta eliminar");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void ActionsData::Spaces::DeleteA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "SET s.state = 'DELETED' " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );

    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::Spaces::DeleteA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("DELETE FROM spaces_users WHERE id_space = ?");

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void ActionsData::Forms::ReadA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Forms::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Forms::ReadSpecificA02::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Forms::AddA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Forms::AddA02::Setup_(Functions::Action::Ptr action)
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
    action_->AddParameter_("state", "", true);
    action_->AddParameter_("privacity", "", true);
    action_->AddParameter_("description", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
}

void ActionsData::Forms::AddA03::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Forms::ModifyA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Forms::ModifyA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id != ? AND id_space = ?");
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
    action_->AddParameter_("space_id", get_space_id(), false);
}

void ActionsData::Forms::ModifyA03::Setup_(Functions::Action::Ptr action)
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
    action_->AddParameter_("state", "", true);
    action_->AddParameter_("privacity", "", true);
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

void ActionsData::Forms::DeleteA01::Setup_(Functions::Action::Ptr action)
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

void ActionsData::Forms::DeleteA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("DELETE FROM forms WHERE id = ? AND id_space = ?");
    action_->AddParameter_("id", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
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