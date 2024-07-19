
#include "web_server.h"

Webserver::Webserver()
{
    // Set headers
    AddHeader_("Access-Control-Allow-Credentials", "true");
    AddHeader_("Access-Control-Allow-Headers", "Content-Type");
    AddHeader_("Access-Control-Allow-Methods", "*");
    AddHeader_("Access-Control-Allow-Origin", "https://localhost:3000");
}

void Webserver::Process_()
{
    SetupHeaders_();
    get_file_manager().AddBasicSupportedFiles_();
    DownloadProcess_();
}