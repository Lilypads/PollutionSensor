#add_library(<name> <wheretofind>)
add_library(Sqr sqr.cpp sqr.h)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps/sps30-i2c)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps/embedded-common)

# main code executable
add_executable(example example.cpp ${PROJECT_SOURCE_DIR}/embedded-sps/sps30-i2c/sps30.h)
target_link_libraries(example Sqr)
