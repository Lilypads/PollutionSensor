# set minimum cmake version
cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# project name
project(PollutionSensor)

# require C++14
set(CMAKE_CXX_STANDARD 14)
# generates commpile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

# enable testing
enable_testing()

# fix for compiler error relating to pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")
#add_library(<name> <wheretofind>)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(ExampleCMakeLib)
add_subdirectory(pigpio)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/SPS30 ${PROJECT_SOURCE_DIR}/src/GPS ${PROJECT_SOURCE_DIR}/src/StorageHandler ${PROJECT_SOURCE_DIR}/src/StateMachine ${PROJECT_SOURCE_DIR}/src/UserInterface)
add_executable(main main.cpp)
target_link_libraries(main libStateMachine libStorageHandler libsps30 neo6m libStateMachine userinterface)

