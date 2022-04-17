include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/SPS30 ${PROJECT_SOURCE_DIR}/src/GPS ${PROJECT_SOURCE_DIR}/src/StorageHandler)

add_executable(stateMachineFunctionsDemo stateMachineFunctionsDemo.cpp)
target_link_libraries(stateMachineFunctionsDemo libStorageHandler libsps30 neo6m)