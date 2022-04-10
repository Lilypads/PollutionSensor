include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/SPS30 ${PROJECT_SOURCE_DIR}/src/GPS)

add_executable(storageHandler storageHandler.cpp storageHandler.h)
target_link_libraries(storageHandler libsps30 neo6m)

