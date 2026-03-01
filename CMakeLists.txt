cmake_minimum_required(VERSION 3.21)

# =============================================================================
# Project definition
# =============================================================================
project(Tetris VERSION 1.0.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================================
# Dependencies
# =============================================================================
find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

# =============================================================================
# Collect sources from sub-directories
# =============================================================================
add_subdirectory(src)

# =============================================================================
# Create the executable (flat build – all sources compiled into one target)
# =============================================================================
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# =============================================================================
# Include directories
# =============================================================================
target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDE_DIRS})

# =============================================================================
# Link SFML
# =============================================================================
target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# =============================================================================
# Platform-specific settings
# =============================================================================
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __APPLE__)
endif()

# =============================================================================
# Compiler warnings
# =============================================================================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =============================================================================
# Debug/Release definitions
# =============================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# =============================================================================
# Copy assets to build output
# =============================================================================
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# =============================================================================
# Installation rules
# =============================================================================
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
