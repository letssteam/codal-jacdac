project(codal-jacdac)

include("${CODAL_UTILS_LOCATION}")


list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/jacdac"
    "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/inc"
)


# file(GLOB C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/*.c")
# file(GLOB C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/interfaces/*.c")
# list(APPEND C_SOURCE_FILES )

# file(GLOB CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/*.c")
# list(APPEND CPP_SOURCE_FILES )

file(GLOB CXX_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/*.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/jacdac/source/interfaces/*.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c??")

# add them
include_directories(${INCLUDE_DIRS})

# create our target
# add_library(codal-jacdac ${C_SOURCE_FILES} ${CPP_SOURCE_FILES})
add_library(codal-jacdac ${CXX_FILES})

target_link_libraries(codal-jacdac codal-core)

# expose it to parent cmake.
target_include_directories(codal-jacdac PUBLIC ${INCLUDE_DIRS})
à