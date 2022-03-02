# set minimum cmake version
cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# project name
project(PollutionSensor)

# require C++14
set(CMAKE_CXX_STANDARD 14)

#add_library(<name> <wheretofind>)
add_subdirectory(src)
add_subdirectory(test)
add_library(Sqr sqr.cpp sqr.h)

# main code executable
add_executable(example example.cpp)
target_link_libraries(example Sqr)
