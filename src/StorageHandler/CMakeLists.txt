add_library(libStorageHandler storageHandler.cpp storageHandler.h)
target_include_directories(libStorageHandler PUBLIC ./)
target_link_libraries(libStorageHandler libsps30 neo6m)
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/SPS30 ${PROJECT_SOURCE_DIR}/src/GPS)

add_executable(storageHandlerDemo storageHandlerDemo.cpp storageHandler.h)
target_link_libraries(storageHandlerDemo libStorageHandler)

