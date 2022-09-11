# remove last end of "/"
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})

# get current relative dir name and set target name
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})
set (PROGRAM_NAME example_${CURRENT_FOLDER})

message(STATUS "the target is ${PROGRAM_NAME}")

aux_source_directory(./ SRC)
add_executable(${PROGRAM_NAME} ${SRC})
target_link_libraries(${PROGRAM_NAME} ${ALL_LIBS})
