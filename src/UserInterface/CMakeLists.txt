find_package(Curses REQUIRED)

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
)
target_include_directories(userinterface PUBLIC ./ ${CURSES_INCLUDE_DIR})
target_link_libraries(userinterface ${CURSES_LIBRARY})


# compile target: demoUserInterface
add_executable(demoUserInterface DemoUserInterface.cpp)
target_link_libraries(demoUserInterface userinterface)
