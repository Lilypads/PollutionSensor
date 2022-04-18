set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

# compile target: userinterface
add_library(
userinterface STATIC
displayHandler.cpp
displayHandler.h
inputHandler.cpp
inputHandler.h
menuOptions.cpp
menuOptions.h
menuHandler.cpp
menuHandler.h
menuData.cpp
menuData.h
)
target_include_directories(userinterface PUBLIC ./ ${CURSES_INCLUDE_DIR})
target_link_libraries(userinterface ${CURSES_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/SPS30 ${PROJECT_SOURCE_DIR}/src/GPS)


# compile target: DemoUserInterface
add_executable(DemoUserInterface DemoUserInterface.cpp)
target_link_libraries(DemoUserInterface userinterface)
