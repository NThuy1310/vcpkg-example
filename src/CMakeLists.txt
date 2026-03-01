# =============================================================================
# src/ – Collect sources from all sub-directories and propagate to top-level
# =============================================================================

# Gather sources and include dirs from each sub-directory
add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(systems)
add_subdirectory(states)

# Aggregate all sources and pass to parent scope
set(ALL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp
    ${CORE_SOURCES}
    ${SYSTEMS_SOURCES}
    ${STATES_SOURCES}
    PARENT_SCOPE
)

# Aggregate all include dirs and pass to parent scope
set(ALL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${UTILS_INCLUDE_DIR}
    ${CORE_INCLUDE_DIR}
    ${SYSTEMS_INCLUDE_DIR}
    ${STATES_INCLUDE_DIR}
    PARENT_SCOPE
)
