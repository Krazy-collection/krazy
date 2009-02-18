include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QT_INCLUDE_DIR}
  ${CPP_PARSER_SOURCE_DIR}
)

set(cpptools_SRCS
  NamePrettyPrinter.cpp
  Overview.cpp
  TypePrettyPrinter.cpp
)

add_library(cpptools SHARED ${cpptools_SRCS})
target_link_libraries(cpptools c++parser)
