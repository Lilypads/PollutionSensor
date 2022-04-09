add_library(libsps30 libsps30.cpp libsps30.h)
target_link_libraries(libsps30 ${pigpio_LIBRARY} ${pigpiod_if_LIBRARY} ${pigpiod_if2_LIBRARY})
target_include_directories(libsps30 PUBLIC ./ ${pigpio_INCLUDE_DIR})


# main code executable
add_executable(sps30demo sps30demo.cpp)
target_link_libraries(sps30demo libsps30)
