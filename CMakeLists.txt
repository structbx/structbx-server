
# General

cmake_minimum_required(VERSION 3.16)
project("structbx-server" LANGUAGES CXX)

# Language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler options
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fstandalone-debug")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add CMake files
include(cmake/version.cmake) 

# Add CMake modules
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
endif()

# Config file
configure_file(${PROJECT_SOURCE_DIR}/cmake/config.h.cmake structbxConfig.h)

# Find libmysqlclient
find_package(libmysqlclient REQUIRED)
if(NOT libmysqlclient_FOUND)
    message(FATAL_ERROR "libmysqlclient not found")
endif()
list(APPEND HEADER ${libmysqlclient_INCLUDE_DIRS_RELEASE})

# Find yaml-cpp
find_package(yaml-cpp REQUIRED)
if(NOT yaml-cpp_FOUND)
    message(FATAL_ERROR "yaml-cpp not found")
endif()
list(APPEND HEADER ${yaml-cpp_INCLUDE_DIRS})

# Find Poco
find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL Util Data DataMySQL JSON)
if(NOT Poco_FOUND)
    message(FATAL_ERROR "Poco not found")
endif()
list(APPEND HEADER ${Poco_INCLUDE_DIRS})

# Dependencies
set(LIB_DEPENDECIES
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::Util
    Poco::Data
    Poco::DataMySQL
    Poco::JSON
    yaml-cpp::yaml-cpp
)

# Header
set(HEADER
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

# Sources
set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/structbx-server.cpp
    ${PROJECT_SOURCE_DIR}/src/web_server.cpp
    ${PROJECT_SOURCE_DIR}/src/backend_server.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/general/main.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/general/users.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/general/groups.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/general/permissions.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/spaces/main.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/spaces/users.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/forms/main.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/forms/data.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/forms/columns.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/forms/permissions.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/id_checker.cpp
)

set(
    SOURCES
    ${SOURCES}
    ${PROJECT_SOURCE_DIR}/src/core/core.cpp
    ${PROJECT_SOURCE_DIR}/src/core/handler_factory.cpp
    ${PROJECT_SOURCE_DIR}/src/core/server.cpp
    ${PROJECT_SOURCE_DIR}/src/files/file_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/files/file_properties.cpp
    ${PROJECT_SOURCE_DIR}/src/files/file.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/action.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/function.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/functions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/backend_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/custom_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/login_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/null_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/handlers/root_handler.cpp
    ${PROJECT_SOURCE_DIR}/src/http/body.cpp
    ${PROJECT_SOURCE_DIR}/src/http/client.cpp
    ${PROJECT_SOURCE_DIR}/src/http/common_responses.cpp
    ${PROJECT_SOURCE_DIR}/src/http/methods.cpp
    ${PROJECT_SOURCE_DIR}/src/http/request.cpp
    ${PROJECT_SOURCE_DIR}/src/query/condition.cpp
    ${PROJECT_SOURCE_DIR}/src/query/database_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/query/field.cpp
    ${PROJECT_SOURCE_DIR}/src/query/parameter.cpp
    ${PROJECT_SOURCE_DIR}/src/query/results.cpp
    ${PROJECT_SOURCE_DIR}/src/query/row.cpp
    ${PROJECT_SOURCE_DIR}/src/security/permission.cpp
    ${PROJECT_SOURCE_DIR}/src/security/permissions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/security/security_verification.cpp
    ${PROJECT_SOURCE_DIR}/src/security/user.cpp
    ${PROJECT_SOURCE_DIR}/src/security/users_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/security/permission.cpp
    ${PROJECT_SOURCE_DIR}/src/sessions/session.cpp
    ${PROJECT_SOURCE_DIR}/src/sessions/sessions_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/dvalue.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/handler_connection.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/hmac_tool.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/id_checker.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/manage_json.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/output_logger.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/route.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/hmac_tool.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/base64_tool.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/settings_manager.cpp
)

# Executable
add_executable(structbx-server ${SOURCES}) 
target_link_libraries(structbx-server PUBLIC ${LIB_DEPENDECIES})
target_include_directories(structbx-server PUBLIC ${HEADER})

# Install files
include(GNUInstallDirs)
install(
    TARGETS structbx-server
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
	FILES ${PROJECT_SOURCE_DIR}/conf/properties.yaml
	DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/structbx/
)
