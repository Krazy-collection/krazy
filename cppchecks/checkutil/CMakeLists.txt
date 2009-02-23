include_directories(
  ${QT_INCLUDE_DIR}
  ${CPP_PREPROCESSOR_SOURCE_DIR}
  ${CPP_PARSER_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
)

set(checkutil_SRCS
  checkengine.cpp
  check.cpp
)
add_library(checkutil SHARED ${checkutil_SRCS})

target_link_libraries(checkutil
    preprocessor
    cpp_parser
)
set_target_properties(checkutil PROPERTIES VERSION 1 SOVERSION 1.0)
install(TARGETS checkutil ${INSTALL_TARGETS_DEFAULT_ARGS})
