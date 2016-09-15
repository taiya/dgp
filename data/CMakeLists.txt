#--- Dummy target just to have data files appear in IDE
file(GLOB_RECURSE DATA "${PROJECT_SOURCE_DIR}/data/*")
add_custom_target(DGP_DATA SOURCES ${DATA})
