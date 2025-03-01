
#ifndef STRUCTBX_TOOLS_OUTPUTLOGGER
#define STRUCTBX_TOOLS_OUTPUTLOGGER


#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <ostream>
#include <chrono>
#include <iomanip>
#include <mutex>


namespace StructBX
{
    namespace Tools
    {
        enum class LogType;
        class OutputLogger;
    }
}


enum class StructBX::Tools::LogType
{
    kInfo
    ,kWarning
    ,kError
    ,kDebug
};

class StructBX::Tools::OutputLogger
{
    public:
        OutputLogger();
        ~OutputLogger(){}

        static bool get_log_to_file() { return log_to_file_; }
        static bool get_print_debug() { return print_debug_; }
        static std::string get_output_file_address() { return output_file_address_; }

        static void set_log_to_file(bool log_to_file) { log_to_file_ = log_to_file; }
        static void set_print_debug(bool print_debug) { print_debug_ = print_debug; }
        static void set_output_file_address(std::string output_file_address) { output_file_address_ = output_file_address; }

        static void Log_(Tools::LogType log_type, std::string& message);
        static void Log_(std::string message);
        static void Warning_(std::string message);
        static void Error_(std::string message);
        static void Debug_(std::string message);
        static std::string CurrentDateTime_();

    private:
        static std::mutex mutex_;
        static bool log_to_file_;
        static bool print_debug_;
        static std::string output_file_address_;
};

#endif // STRUCTBX_TOOLS_OUTPUTLOGGER
