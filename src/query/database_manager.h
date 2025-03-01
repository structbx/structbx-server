
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

#include "tools/settings_manager.h"

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
        DatabaseManager();
        ~DatabaseManager();

        static void StartMySQL_();
        static void StopMySQL_();
        static std::shared_ptr<Data::Session> StartSessionMySQL_();

    private:
        static bool initialized_;

};

#endif // STRUCTBX_QUERY_DATABASEMANAGER
