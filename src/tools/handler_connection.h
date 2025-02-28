// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef STRUCTBX_TOOLS_HANDLERCONNECTION
#define STRUCTBX_TOOLS_HANDLERCONNECTION


#include <functional>

#include "handlers/root_handler.h"
#include "handlers/null_handler.h"
#include "handlers/backend_handler.h"


namespace StructBX
{
    namespace Tools
    {
        class HandlerConnection;
    }
}

class StructBX::Tools::HandlerConnection
{
    public:
        using HandlerFunctor = std::function<StructBX::Handlers::RootHandler*()>;

        HandlerConnection(StructBX::Tools::Route route, HandlerFunctor handler);
        ~HandlerConnection();

        StructBX::Tools::Route current_route_;
        HandlerFunctor return_handler_;
};


#endif // STRUCTBX_TOOLS_HANDLERCONNECTION
