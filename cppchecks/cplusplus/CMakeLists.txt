add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(cppmodel)
add_subdirectory(parser)
add_subdirectory(preprocessor)
