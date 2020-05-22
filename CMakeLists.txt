project(malloctest)
set(CMAKE_C_FLAGS "-std=gnu11 -Wall -Wno-long-long -pedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set source files and include directories
file(GLOB SOURCES 
    main.c
    src/mmemory.c
)
include_directories(
    ${CMAKE_BINARY_DIR}/include
)

# add executable
add_executable(malloctest "${SOURCES}")