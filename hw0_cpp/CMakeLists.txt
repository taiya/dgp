get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${LIBRARIES})

#================================
# Source groups (????)
#================================
#source_group("source" FILES ${SOURCES})
