add_subdirectory(GPS)
add_subdirectory(SPS30)
add_subdirectory(UserInterface)
add_subdirectory(StorageHandler)
add_subdirectory(GitWrapper)
add_subdirectory(StateMachine)

add_executable(main main.cpp)
target_link_libraries(main libStateMachine libStorageHandler libsps30 neo6m libStateMachine userinterface)
