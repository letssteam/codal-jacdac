project(codal-jacdac)

include("${CODAL_UTILS_LOCATION}")


list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/jacdac"
    "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/inc"
)


RECURSIVE_FIND_FILE(CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/" "*.c??")
list(APPEND CPP_SOURCE_FILES
)


# RECURSIVE_FIND_FILE(C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/" "*.c")
# file(GLOB C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/interfaces/*.c")
file(GLOB C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/*.c")
list(APPEND C_SOURCE_FILES )

# add them
include_directories(${INCLUDE_DIRS})

# create our target
add_library(codal-jacdac ${C_SOURCE_FILES} ${CPP_SOURCE_FILES})

target_link_libraries(codal-jacdac codal-core)

# expose it to parent cmake.
target_include_directories(codal-jacdac PUBLIC ${INCLUDE_DIRS})
