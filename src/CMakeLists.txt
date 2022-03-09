#add_library(<name> <wheretofind>)
add_library(Sqr sqr.cpp sqr.h)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps)

# main code executable
add_executable(example example.cpp)
target_include_directories(example ${PROJECT_SOURCE_DIR}/embedded-sps)
target_link_libraries(example Sqr)
