
#include <iostream>
#include <memory>

#include <Poco/File.h>
#include <Poco/Exception.h>

#include "core/core.h"
#include "functions/action.h"
#include "tools/route.h"
#include "query/parameter.h"
#include "tools/dvalue.h"
#include "tools/output_logger.h"
#include "tools/hmac_tool.h"


using namespace Poco;
using namespace StructBX;

struct Parameters
{
    std::string properties_file = "";
};

Parameters SetupParameters(std::vector<std::string>& parameters)
{
    Parameters params;

    // Properties file
    auto config = std::find(parameters.begin(), parameters.end(), "--config");
    params.properties_file = config != parameters.end() ? *(config + 1) : "properties.yaml";

    // Remove all parameters except the first one
    std::string first_param = parameters.front();
    parameters = std::vector<std::string>();
    parameters.push_back(first_param);

    return params;
}

int main(int argc, char** argv)
{
    // Setup
        Core::Core app(true);

    // Parameters
        auto& parameters = app.get_console_parameters();
        parameters = std::vector<std::string>(argv, argv + argc);
        Parameters params = SetupParameters(parameters);

    // Settings
        StructBX::Tools::SettingsManager::set_config_path(params.properties_file);
        StructBX::Tools::SettingsManager::ReadSettings_();
        app.SetupSettings_();

    // Setup
        StructBX::Query::DatabaseManager::StartMySQL_();
        StructBX::Security::PermissionsManager::LoadPermissions_();
        StructBX::Tools::SessionsManager::ReadSessions_();

    // Run
        auto code = app.Init_();

    // End
        StructBX::Query::DatabaseManager::StopMySQL_();
        return code;

}
