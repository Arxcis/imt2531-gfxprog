cmake_minimum_required(VERSION 3.1)
project(cube)

# Add other cmake projects to this project
add_subdirectory(${CMAKE_BINARY_DIR}/lib/glfw)
add_subdirectory(${CMAKE_BINARY_DIR}/lib/glew/build/cmake)

add_executable(cube src/main.cpp)
set_target_properties(cube
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    COMPILE_FLAGS "-g -Wall -Wextra")

target_sources(cube
    PRIVATE ${CMAKE_BINARY_DIR}/src/Shader.cpp
    PRIVATE ${CMAKE_BINARY_DIR}/src/Mesh.cpp)

target_include_directories(cube
    PRIVATE ${CMAKE_BINARY_DIR}/lib
    PRIVATE ${CMAKE_BINARY_DIR}/lib/glm
    PRIVATE ${CMAKE_BINARY_DIR}/include)

target_link_libraries(cube 
    glew ${GLEW_LIBRARIES} 
    glfw ${GLFW_LIBRARIES})

target_compile_features(cube 
    PRIVATE cxx_auto_type
    PRIVATE cxx_range_for
    PRIVATE cxx_generic_lambdas)



#
# DEBUG MESSAGES
#
message("GLEW_LIBRARIES" ${GLEW_LIBRARIES})
message("GLFW_LIBRARIES" ${GLFW_LIBRARIES})
message("glfw_INCLUDE_DIRS" ${glfw_INCLUDE_DIRS})

message("++ CMAKE_BINARY_DIR" ${CMAKE_BINARY_DIR})
message("++ system name is: " ${CMAKE_SYSTEM} )
message("++ system processor is: " ${CMAKE_SYSTEM_PROCESSOR} )
