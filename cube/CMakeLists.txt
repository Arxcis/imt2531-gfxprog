cmake_minimum_required(VERSION 3.1)
project(cube)
set(ROOT ${CMAKE_BINARY_DIR})

#
# INCLUDE
#
# System libraries
find_package(GLEW REQUIRED)
#find_package(OpenGL REQUIRED)
add_subdirectory(${ROOT}/lib/glfw-3.2.1)

#
# CONFIGURE EXECUTEABLE
#
set(TARGET ${CMAKE_PROJECT_NAME})
add_executable(${TARGET} src/main.cpp)

set_target_properties(${TARGET}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${ROOT}/lib"
    BUILD_TYPE "Debug"              # Debug, Release, RelWithDebInfo and MinSizeRel
    COMPILE_FLAGS "-Wall      \
                   -Wno-deprecated-declarations")

target_sources(${TARGET}
    PRIVATE ${ROOT}/src/load.cpp
    PRIVATE ${ROOT}/src/render.cpp
    PRIVATE ${ROOT}/src/mesh.cpp)

target_include_directories(${TARGET}
    PRIVATE ${ROOT}/lib
    PRIVATE ${ROOT}/lib/glm
    PRIVATE ${ROOT}/include)

target_link_libraries(${TARGET}
    PRIVATE glew ${GLFW_LIBRARIES}
    PRIVATE glfw ${GLFW_LIBRARIES})

target_compile_features(${TARGET}
    PRIVATE cxx_auto_type
    PRIVATE cxx_range_for
    PRIVATE cxx_generic_lambdas)
