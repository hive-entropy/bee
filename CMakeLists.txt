cmake_minimum_required(VERSION 3.17)
project(PDS_Bee)
set(CMAKE_CXX_STANDARD 14)

# Add library's directory
add_subdirectory(API)

# Add test subdirectory
add_subdirectory(test)

# Add includes and sources
set(SOURCE_FILES src/main.cpp)
include_directories(include)

# Create the executable
add_executable(PDS_Bee ${SOURCE_FILES})

# Add the library's headers
include_directories(API/heapi/include)
# Link the library
target_link_libraries(PDS_Bee heapi)