#add_library(<name> <wheretofind>)
add_library(Sqr sqr.cpp sqr.h)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps/sps30-i2c)
include_directories(${PROJECT_SOURCE_DIR}/embedded-sps/embedded-common)

# main code executable
#add_executable(example example.cpp ${PROJECT_SOURCE_DIR}/embedded-sps/sps30-i2c/sps30.h)
#target_link_libraries(example Sqr)

# try sps30manager
find_library(bcm2835_LIB bcm2835 REQUIRED)
add_library(Sps30 sps30lib.cpp sps30lib.h)
add_executable(sps30manager sps30manager.cpp)
target_link_libraries(sps30manager Sps30 ${BCM2835_LIB} ${BCM2835_INCLUDE_DIR})