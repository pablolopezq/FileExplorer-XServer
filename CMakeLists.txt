cmake_minimum_required (VERSION 3.0)
project (file_explorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
                    src/main.cpp
                    src/FilesystemOps.cpp
                    src/App.cpp)

target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++ -L/usr/X11/lib -lX11 -lstdc++)