cmake_minimum_required(VERSION 3.10)
project(TP-VC)

set(CMAKE_CXX_STANDARD 11)

# Include OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include current source directory for headers
include_directories(${CMAKE_SOURCE_DIR})

# Add your source files here
add_executable(TP-VC main.cpp vc.c bib.c)

# Link OpenCV
target_link_libraries(TP-VC ${OpenCV_LIBS})
