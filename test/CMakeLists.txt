##############################################
# Import external dependencies

# Create lib folder if it doesn't exist
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/test/lib)
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/test/lib)
    message(STATUS "Created lib directory in ${PROJECT_SOURCE_DIR}/test")
endif()

function(download_library LIB_NAME URL DIRECTORY)
    # Download the required library in given folder
    message(STATUS "Downloading library: ${LIB_NAME}")

    # Extract file_name from url. The url must end with the header file with 'hpp' extension.
    string(REGEX MATCH "[^/]+\.hpp$" FILE_NAME ${URL})

    if(EXISTS ${DIRECTORY}/${FILE_NAME})
        message(STATUS "Library already exists. Download skipped.")
        return()
    endif()

    set(CMD_ARGS "-LO" ${URL})
    execute_process(COMMAND "curl" ${CMD_ARGS}
            WORKING_DIRECTORY ${DIRECTORY}
            RESULT_VARIABLE CURL_RESULT)

    # Print download status
    if(${CURL_RESULT} EQUAL 0)
        message(STATUS "Library successfully downloaded")
    else()
        message(STATUS "Can't download library: ${CURL_RESULT}")
    endif()
endfunction()

download_library(FakeIt
        "https://raw.githubusercontent.com/eranpeer/FakeIt/master/single_header/catch/fakeit.hpp"
        ${PROJECT_SOURCE_DIR}/test/lib)

download_library(Catch2
        "https://raw.githubusercontent.com/catchorg/Catch2/v2.x/single_include/catch2/catch.hpp"
        ${PROJECT_SOURCE_DIR}/test/lib)


##############################################
# Create test target

# Create a library target for the Catch and FakeIt header-only test frameworks.
add_library(Catch_FakeIt INTERFACE)
target_include_directories(Catch_FakeIt
        INTERFACE lib
        )

add_executable (BeeTest
        src/test.cpp
        src/bee_test.cpp
        src/endpoint_test.cpp
        src/processor_test.cpp
)

target_link_libraries(BeeTest
        PRIVATE
            BeeLib
            Catch_FakeIt
        )

