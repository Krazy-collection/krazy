set(cpptools_SRCS
  NamePrettyPrinter.cpp
  Overview.cpp
  TypePrettyPrinter.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(cpptools SHARED ${cpptools_SRCS})
target_link_libraries(cpptools c++parser)
