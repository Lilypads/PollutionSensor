add_library(neo6m STATIC neo6m.cpp neo6m.h)
target_include_directories(neo6m PUBLIC ./)


# main code executable
add_executable(neo6mDemo neo6mDemo.cpp)
target_link_libraries(neo6mDemo neo6m)
