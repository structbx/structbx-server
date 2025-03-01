
#ifndef STRUCTBX_QUERY_DATABASEMANAGER
#define STRUCTBX_QUERY_DATABASEMANAGER


#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "Poco/Exception.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>


namespace StructBX
{
    namespace Query
    {
        class DatabaseManager;
    }
}

using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

class StructBX::Query::DatabaseManager
{
    public:
        struct Credentials
        {
            Credentials() :
                db_host("")
                ,db_port("")
                ,db_name("")
                ,db_user("")
                ,db_password("")
            {

            }
            Credentials(std::string db_host, std::string db_port, std::string db_name, std::string db_user, std::string db_password) :
                db_host(db_host)
                ,db_port(db_port)
                ,db_name(db_name)
                ,db_user(db_user)
                ,db_password(db_password)
            {

            }
            void Replace_(Credentials& credentials)
            {
                db_host = credentials.db_host;
                db_port = credentials.db_port;
                db_name = credentials.db_name;
                db_user = credentials.db_user;
                db_password = credentials.db_password;
            }

            std::string db_host, db_port, db_name, db_user, db_password;
        };

        DatabaseManager();
        ~DatabaseManager();

        static void StartMySQL_();
        static void StopMySQL_();
        static std::shared_ptr<Data::Session> StartSessionMySQL_(Credentials& credentials);

    private:
        static bool initialized_;

};

#endif // STRUCTBX_QUERY_DATABASEMANAGER
