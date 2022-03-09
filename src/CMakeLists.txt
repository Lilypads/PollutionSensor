#add_library(<name> <wheretofind>)
add_library(Sqr sqr.cpp sqr.h)
include_directory(embedded-sps)

# main code executable
add_executable(example example.cpp)
target_link_libraries(example Sqr)
