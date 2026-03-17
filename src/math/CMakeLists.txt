get_filename_component(NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

project(${NAME})
aux_source_directory("${PROJECT_SOURCE_DIR}" SOURCES)
add_library("${PROJECT_NAME}" STATIC ${SOURCES})
