#add_library(<name> <wheretofind>)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

#ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/GPS ${PROJECT_SOURCE_DIR}/ExampleCMakeLib)
#add_definitions (-DBOOST_TEST_DYN_LINK)


# test1 (example) ################################################################################
# testing executable
add_executable(test1 test1.cpp) # ${TEST_SOURCE_DIR}/src/sqr.cpp)
# here we are adding the libraries we want to link the combiled binary test1 to
target_link_libraries(test1 Sqr ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})
# add tests
add_test(NAME mytest1 COMMAND test1)


# NEO6M ################################################################################
# testing executable
add_executable(neo6m_test neo6m_test.cpp) # ${TEST_SOURCE_DIR}/src/neo6m_test.cpp)
# here we are adding the libraries we want to link the combiled binary test1 to
target_link_libraries(neo6m_test neo6m ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})

# add tests
add_test(NAME mytest_neo6m COMMAND neo6m_test)
