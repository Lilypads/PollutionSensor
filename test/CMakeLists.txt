#add_library(<name> <wheretofind>)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/ExampleCMakeLib)
#add_definitions (-DBOOST_TEST_DYN_LINK)

# testing executable
add_executable(test_sps30Lib test_sps30Lib.cpp )         # ${TEST_SOURCE_DIR}/src/sqr.cpp)

# here we are adding the libraries we want to link the combiled binary test_sps30Lib to
target_link_libraries(test_sps30Lib libsps30 ${Boost_LIBRARIES} ${BOOST_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMWORK_LIBRARY})


# enable testing
enable_testing()

# add tests
#add_test(NAME sps30test COMMAND test_sps30Lib)
