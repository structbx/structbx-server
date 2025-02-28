/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2023  Jose F Rivas C <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "query/database_manager.h"

using namespace NAF;
using namespace NAF::Query;

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
