
#ifndef STRUCTBX_TOOLS_SETTINGSMANAGER
#define STRUCTBX_TOOLS_SETTINGSMANAGER


#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include <algorithm>

#include "tools/dvalue.h"
#include "yaml-cpp/node/detail/iterator_fwd.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/yaml.h"
#include "Poco/Exception.h"

#include "tools/dvalue.h"

namespace StructBX
{
    namespace Tools
    {
        class SettingsManager;
    }
}

using namespace Poco;


class StructBX::Tools::SettingsManager
{
    public:
        struct Setting
        {
            Setting(std::string name, Tools::DValue::Type type, Tools::DValue value) :
                name(name)
                ,type(type)
                ,value(value)
            {

            }
            std::string name;
            Tools::DValue::Type type;
            Tools::DValue value;
        };

        SettingsManager();

        static std::vector<Setting>& get_settings()
        {
            auto& var = settings_;
            return var;
        }
        static std::string get_config_path() { return config_path_; }

        static void set_config_path(std::string config_path) { config_path_ = config_path; }

        static std::vector<Setting>::iterator GetSetting_(std::string setting_name);
        static std::string GetSetting_(std::string setting_name, const char* another_value);
        static std::string GetSetting_(std::string setting_name, std::string another_value);
        static int GetSetting_(std::string setting_name, int another_value);
        static float GetSetting_(std::string setting_name, float another_value);
        static bool GetSetting_(std::string setting_name, bool another_value);
        static void AddSetting_(std::string name, Tools::DValue::Type type, Tools::DValue value);
        static void AddBasicSettings_();
        static void ReadSettings_();

    protected:
        static bool VerifyYAMLScalarNode_(YAML::Node& node);
        static bool VerifyYAMLMapNode_(YAML::Node& node);
        static void PrintError_(std::string function, std::string variable);

    private:
        static std::mutex mutex_;
        static std::vector<Setting> settings_;
        static std::string config_path_;
};

#endif // STRUCTBX_TOOLS_SETTINGSMANAGER
