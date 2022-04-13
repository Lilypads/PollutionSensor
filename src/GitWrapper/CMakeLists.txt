# find_package(libgit2 REQUIRED)
add_library(gitwrapper gitWrapper.cpp gitWrapper.h)
target_link_libraries(gitwrapper )#${libgit2_LIBRARIES})
target_include_directories(gitwrapper PUBLIC ./ )#${libgit2_INCLUDE_DIRS})


# main code executable
add_executable(DemoGitWrapper DemoGitWrapper.cpp)
target_link_libraries(DemoGitWrapper gitwrapper)
