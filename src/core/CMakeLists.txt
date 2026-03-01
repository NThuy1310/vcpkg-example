# =============================================================================
# core – Collect source files
# =============================================================================
set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Board.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PieceController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ScoreManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TetrominoFactory.cpp
    PARENT_SCOPE
)

set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
