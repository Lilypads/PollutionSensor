find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)


include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/GPS ${PROJECT_SOURCE_DIR}/ExampleCMakeLib)


# NEO6M ################################################################################
# testing executable
add_executable(neo6m_test neo6m_test.cpp) 
# here we are adding the libraries we want to link the combiled binary test1 to
target_link_libraries(neo6m_test neo6m ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})

# add tests
add_test(NAME mytest_neo6m COMMAND neo6m_test)


# SPS30 ################################################################################
# testing executable
add_executable(test_sps30Lib test_sps30Lib.cpp ) 
# here we are adding the libraries we want to link the combiled binary test_sps30Lib to
target_link_libraries(test_sps30Lib libsps30 ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})


# enable testing
enable_testing()

# add tests
#add_test(NAME sps30test COMMAND test_sps30Lib)
