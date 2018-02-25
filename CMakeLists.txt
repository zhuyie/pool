cmake_minimum_required(VERSION 2.6)

project(pool)

set(INCLUDE_FILES
    pool.h
)

set(SOURCE_FILES
    pool.c
)

source_group(include FILES ${INCLUDE_FILES})
source_group(source FILES ${SOURCE_FILES})

add_library(pool STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

include_directories(".")
add_executable(testapp test/test.c)
target_link_libraries(testapp pool)
