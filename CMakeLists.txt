# set minimum cmake version
cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# project name
project(PollutionSensor)

# require C++14
set(CMAKE_CXX_STANDARD 14)
# generates commpile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

# fix for compiler error relating to pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#add_library(<name> <wheretofind>)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)
add_subdirectory(ExampleCMakeLib)
add_subdirectory(pigpio)
