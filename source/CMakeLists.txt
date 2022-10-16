file(GLOB_RECURSE SOURCES "*.cpp")

set(CMAKE_CXX_STANDARD 11)

add_executable(scc main.cxx ${SOURCES})
set_target_properties(scc PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED OFF
        CXX_EXTENSIONS OFF
)


add_executable(token_test token_test.cxx ${SOURCES})
set_target_properties(token_test PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED OFF
        CXX_EXTENSIONS OFF
)