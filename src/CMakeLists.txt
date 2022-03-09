#add_library(<name> <wheretofind>)
add_library(Sqr sqr.cpp sqr.h)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps/sps30-i2c)

# main code executable
add_executable(example example.cpp sps30.h)
target_link_libraries(example Sqr)
