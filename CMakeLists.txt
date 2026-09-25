cmake_minimum_required(VERSION 3.15)
project(ForgeLite VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 1. Configure GLFW options BEFORE adding its directory
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")

# 2. Tell CMake to step inside the GLFW source directory and build it statically
add_subdirectory(3rdparty/glfw)

# 3. Create your Core Framework Library target
add_library(forge_lite STATIC 
    src/glad.c
    src/main.c           # <--- Added your main implementation file here
    $<TARGET_OBJECTS:glfw>
)


# 4. Expose include paths internally for compiling
target_include_directories(forge_lite PUBLIC include)
target_include_directories(forge_lite PRIVATE 3rdparty/glfw/include)

# 5. Platform Specific Options: Link standard system graphics packages
if(WIN32)
    target_link_libraries(forge_lite PUBLIC gdi32)
else()
    target_link_libraries(forge_lite PUBLIC m pthread dl)
endif()

# 6. Build the test executable from test.c
# Adding WIN32 hides the console window on Windows builds
add_executable(test_app WIN32 test.c)


# Link your forge_lite framework target directly into the test application
target_link_libraries(test_app PRIVATE forge_lite)