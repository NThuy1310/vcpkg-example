# =============================================================================
# systems – Collect source files
# =============================================================================
set(SYSTEMS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FontManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/InputHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SettingsManager.cpp
    PARENT_SCOPE
)

set(SYSTEMS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
