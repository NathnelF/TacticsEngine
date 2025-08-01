cmake_minimum_required(VERSION 3.16)
project(TacticalGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find raylib
find_package(raylib REQUIRED)

# Define directories
set(IMGUI_DIR external/imgui)
set(RLIMGUI_DIR external/rlImGui)

# ImGui source files
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# rlImGui source files
set(RLIMGUI_SOURCES
    ${RLIMGUI_DIR}/rlImGui.cpp
)

# Create ImGui library
add_library(imgui_lib STATIC ${IMGUI_SOURCES})
target_include_directories(imgui_lib PUBLIC ${IMGUI_DIR})

# Create rlImGui library
add_library(rlimgui_lib STATIC ${RLIMGUI_SOURCES})
target_include_directories(rlimgui_lib PUBLIC 
    ${RLIMGUI_DIR}
    ${IMGUI_DIR}
)
# rlImGui depends on ImGui and Raylib
target_link_libraries(rlimgui_lib PUBLIC imgui_lib raylib)

# Collect your source files from src directory
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Collect header files for IDE organization
file(GLOB_RECURSE HEADERS 
    "src/*.hpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    raylib
    imgui_lib
    rlimgui_lib
    m
)

# Include directories for your project
target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    ${IMGUI_DIR}
    ${RLIMGUI_DIR}
)

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Optional: Set some ImGui compile definitions if needed
target_compile_definitions(imgui_lib PRIVATE
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
