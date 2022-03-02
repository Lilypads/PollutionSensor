#add_library(<name> <wheretofind>)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} ${EXAMPLE_SOURCE_DIR}/src)
#add_definitions (-DBOOST_TEST_DYN_LINK)

# testing executable
add_executable(test1 test1.cpp)         #${TEST_SOURCE_DIR}/src/sqr.cpp)
target_link_libraries(test1 Sqr ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})

# add tests
add_test(NAME mytest1 COMMAND test1)
