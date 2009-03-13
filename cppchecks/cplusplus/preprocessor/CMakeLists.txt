include_directories(
  ${QT_INCLUDE_DIR}
  ${CPP_PARSER_SOURCE_DIR}
)

set(preproc_SRCS
    Macro.cpp
    pp-engine.cpp
    pp-macro-expander.cpp
    pp-scanner.cpp
    PreprocessorClient.cpp
    PreprocessorEnvironment.cpp
)

add_library(preprocessor SHARED ${preproc_SRCS})

target_link_libraries(preprocessor cpp_parser ${QT_QTCORE_LIBRARY})
set_target_properties(preprocessor PROPERTIES DEFINE_SYMBOL CPLUSPLUSPREPROCESSOR_BUILD_LIB)

if(WIN32)
install(TARGETS preprocessor ${INSTALL_TARGETS_DEFAULT_ARGS})
endif(WIN32)

