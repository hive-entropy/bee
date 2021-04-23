cmake_minimum_required(VERSION 3.17)
project(PDS_Bee)

##############################################
# Find system dependencies
find_package(HiveEntropyAPI REQUIRED)

##############################################
# Create target and set properties

# Create the executable
add_library(BeeLib
        src/bee.cpp
        src/endpoint.cpp
        src/processor.cpp
        )
add_executable(Bee
        src/main.cpp
        )

# Add header files
target_include_directories(BeeLib PUBLIC include)
target_include_directories(Bee PUBLIC include)

# Set compiler options
target_compile_options(BeeLib PRIVATE -Wall)
target_compile_features(BeeLib PRIVATE cxx_std_11)

# Link the library
target_link_libraries(BeeLib
        PRIVATE
            HiveEntropyAPI::HiveEntropyAPI
)

target_link_libraries(Bee BeeLib)

##############################################
# Installation instructions

##############################################
## Add tests
# Test integration: https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
add_subdirectory(test)