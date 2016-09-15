#--- OpenGP (headeronly)
find_package(OpenGP REQUIRED)
include_directories(${OpenGP_INCLUDE_DIR})
#message(STATUS ${OpenGP_INCLUDE_DIR})

#--- Use header only implementation
add_definitions(-DOPENGP_HEADERONLY)   

# avoid errors caused by duplicates
if(NOT TARGET OPENGP) 
    #--- Dummy target just to have sources/headers appear in IDE
    file(GLOB_RECURSE SOURCES "${OpenGP_INCLUDE_DIR}/*.cpp")
    file(GLOB_RECURSE HEADERS "${OpenGP_INCLUDE_DIR}/*.h")
    add_custom_target(OPENGP SOURCES ${HEADERS} ${SOURCES})
endif()

#--- On UNIX|APPLE you can do "make opengp" to update the library
#if(CMAKE_HOST_UNIX)
#    add_custom_target(opengp_update
#        COMMAND rm -rf OpenGP
#        COMMAND svn export https://github.com/OpenGP/OpenGP/trunk/src/OpenGP
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#        COMMENT "Updating OpenGP ${PROJECT_SOURCE_DIR}/OpenGP")
#endif()
