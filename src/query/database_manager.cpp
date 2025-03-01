
#include "query/database_manager.h"

using namespace StructBX::Query;

bool DatabaseManager::initialized_ = false;

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::StartMySQL_()
{
    Data::MySQL::Connector::registerConnector();
    initialized_ = true;
}

void DatabaseManager::StopMySQL_()
{
    if(initialized_)
        Data::MySQL::Connector::unregisterConnector();
}

std::shared_ptr<Data::Session> DatabaseManager::StartSessionMySQL_()
{
    auto dbconn = std::make_shared<Data::Session>
    (
        "MySQL"
        ,"host=" + Tools::SettingsManager::GetSetting_("db_host", "127.0.0.1")
            + ";port=" + Tools::SettingsManager::GetSetting_("db_port", "3306")
            + ";db=" + Tools::SettingsManager::GetSetting_("db_name", "")
            + ";user=" + Tools::SettingsManager::GetSetting_("db_user", "")
            + ";password=" + Tools::SettingsManager::GetSetting_("db_password", "") + ";"
    );

    return dbconn;
}
