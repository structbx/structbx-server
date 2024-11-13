
# General

cmake_minimum_required(VERSION 3.16)
project("structbi-server" LANGUAGES CXX)

# Language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler options
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fstandalone-debug")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add CMake modules
include(GNUInstallDirs)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extra_modules)
endif()

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

# Find NebulaAtom
find_package(nebulaatom REQUIRED)
if(NOT nebulaatom_FOUND)
    message(FATAL_ERROR "nebulaatom not found")
endif()
list(APPEND HEADER ${nebulaatom_INCLUDE_DIRS})

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
    nebulaatom::nebulaatom
)

# Header
set(HEADER
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

# Sources
set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/structbi_server.cpp
    ${PROJECT_SOURCE_DIR}/src/web_server.cpp
    ${PROJECT_SOURCE_DIR}/src/backend_server.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/organizations/main.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/organizations/users.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/spaces/main.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/spaces/users.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/forms/main.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/forms/data.cpp
    ${PROJECT_SOURCE_DIR}/src/functions/forms/columns.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/actions_data.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/id_checker.cpp
)

# Executable
add_executable(structbi-server ${SOURCES}) 
target_link_libraries(structbi-server PUBLIC ${LIB_DEPENDECIES})
target_include_directories(structbi-server PUBLIC ${HEADER})

# Copy properties file
file(COPY ${CMAKE_SOURCE_DIR}/properties.yaml
	DESTINATION ${CMAKE_BINARY_DIR}
)