#add_library(<name> <wheretofind>)
#add_library(Sqr sqr.cpp sqr.h)

# main code executable
#add_executable(example example.cpp)
#target_link_libraries(example Sqr)


add_subdirectory(GPS)
add_library(Try1 try1.cpp try1.h)
#add_subdirectory(GPS)

# main code executable
add_executable(example example.cpp)
target_link_libraries(example Try1 pigpio)
