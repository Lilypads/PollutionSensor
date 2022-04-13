add_library(
userinterface STATIC
displayHandler.cpp
displayHandler.h
inputHandler.cpp
inputHandler.h
menuHandler.cpp
menuHandler.h
)
target_include_directories(neo6m PUBLIC ./)
