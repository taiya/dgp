get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${LIBRARIES})
include_directories(internal)

#--- Deploy data files
file(COPY ${PROJECT_SOURCE_DIR}/data/indorelax.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
