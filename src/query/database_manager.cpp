
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

std::shared_ptr<Data::Session> DatabaseManager::StartSessionMySQL_(Credentials& credentials)
{
    return std::make_shared<Data::Session>
    (
        "MySQL"
        ,"host=" + credentials.db_host
            + ";port=" + credentials.db_port
            + ";db=" + credentials.db_name
            + ";user=" + credentials.db_user
            + ";password=" + credentials.db_password + ";"
    );
}
