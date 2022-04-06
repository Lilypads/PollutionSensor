
add_library(neo6m neo6m.cpp neo6m.h)


# main code executable
add_executable(neo6mExample neo6mExample.cpp)
target_link_libraries(neo6mExample neo6m)
