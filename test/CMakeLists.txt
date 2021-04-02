# Create lib folder if it doesn't exist
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/test/lib)
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/test/lib)
    message(STATUS "Created lib directory in ${PROJECT_SOURCE_DIR}/test")
endif()

# Download the required libraries in lib folder
message(STATUS "Downloading test libraries")
set(CMD_ARGS "-LO" "https://raw.githubusercontent.com/eranpeer/FakeIt/master/single_header/catch/fakeit.hpp"
        "-LO" "https://raw.githubusercontent.com/catchorg/Catch2/v2.x/single_include/catch2/catch.hpp")
execute_process(COMMAND "curl" ${CMD_ARGS}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/lib
        RESULT_VARIABLE CURL_RESULT)

# Print download status
if(${CURL_RESULT} EQUAL 0)
    message(STATUS "Test libraries successfully downloaded")
else()
    message(STATUS "Can't download libraries: ${CURL_RESULT}")
endif()
