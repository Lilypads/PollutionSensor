#add_library(<name> <wheretofind>)
#add_library(Sqr sqr.cpp sqr.h)

# main code executable
#add_executable(example example.cpp)
#target_link_libraries(example Sqr)


add_library(libsps30 libsps30.cpp libsps30.h)
target_link_libraries(libsps30 pigpio)

# main code executable
add_executable(sps30demo sps30demo.cpp)
target_link_libraries(sps30demo libsps30 pigpio)
