# set minimum cmake version
cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# project name
project(PollutionSensor)

# require C++14
set(CMAKE_CXX_STANDARD 14)

#add_library(<name> <wheretofind>)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
#add_definitions (-DBOOST_TEST_DYN_LINK)
add_library(Sqr sqr.cpp sqr.h)

# main code executable
add_executable(example example.cpp sqr.h)
target_link_libraries(example Sqr)

# testing executable
add_executable(test1 test1.cpp)
target_link_libraries(test1 Sqr ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})

#add_executable(test2 test2.cpp)
#target_link_libraries(test2 ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})

# enable testing
enable_testing()

# add tests
add_test(NAME mytest1 COMMAND test1)
#add_test(NAME mytest2 COMMAND $<TARGET_FILE:test2>)
