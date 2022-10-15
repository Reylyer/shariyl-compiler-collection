file(GLOB_RECURSE SOURCES "*.cpp")

set(TARGET scc)
add_executable(${TARGET} ${SOURCES})
set_target_properties(scc PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED OFF
        CXX_EXTENSIONS OFF
)