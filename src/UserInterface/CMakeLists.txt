# compile target: userinterface
add_library(
userinterface STATIC
displayHandler.cpp
displayHandler.h
inputHandler.cpp
inputHandler.h
menuHandler.cpp
menuHandler.h
)
target_include_directories(userinterface PUBLIC ./)


# compile target: demoUserInterface
add_executable(demoUserInterface DemoUserInterface.cpp)
target_link_libraries(demoUserInterface userinterface)
