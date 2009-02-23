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

set_target_properties(cpptools PROPERTIES DEFINE_SYMBOL CPLUSPLUSTOOLS_BUILD_LIB)
target_link_libraries(cpptools cpp_parser ${QT_QTCORE_LIBRARY})

if(WIN32)
install(TARGETS cpptools ${INSTALL_TARGETS_DEFAULT_ARGS})
endif(WIN32)

