cmake_minimum_required(VERSION 3.17)
project(PDS_Bee)

##############################################
# Find system dependencies
find_package(HiveEntropyAPI)

##############################################
# Create target and set properties

# Add source files
set(SOURCE_FILES
        src/main.cpp
        src/Bee.cpp
        src/Endpoint.cpp
        src/Processor.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add header files
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Set compiler options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)

# Link the library
target_link_libraries(${PROJECT_NAME} PRIVATE HiveEntropyAPI::HiveEntropyAPI)

##############################################
# Installation instructions

##############################################
## Add test
enable_testing()
add_subdirectory(test)