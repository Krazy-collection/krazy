include_directories(
  ${QT_INCLUDE_DIR}
  ${CPP_PREPROCESSOR_SOURCE_DIR}
  ${CPP_PARSER_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
)

add_executable(cpp-test cpp-test.cpp)

target_link_libraries(cpp-test
    preprocessor
    cpp_parser
)
