# minimum required version of cmake
cmake_minimum_required (VERSION 3.8)

# information of the project
project (Lab6)

# set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# set the output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# set the object
add_executable(main src/main.cpp src/Huffman.cpp)

# set the output path of object file
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})