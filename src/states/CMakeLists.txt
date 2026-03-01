# =============================================================================
# states – Collect source files
# =============================================================================
set(STATES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GameOverState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HighScoreState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MenuState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PauseState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PlayState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SettingsState.cpp
    PARENT_SCOPE
)

set(STATES_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
