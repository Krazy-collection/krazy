include_directories(
  ${QT_INCLUDE_DIR}
  ${CPP_PARSER_SOURCE_DIR}
)

set(preproc_SRCS
    CppDocument.cpp
    CppPreprocessor.cpp
    Macro.cpp
    pp-engine.cpp
    pp-macro-expander.cpp
    pp-scanner.cpp
    PreprocessorClient.cpp
    PreprocessorEnvironment.cpp
)

add_library(preprocessor SHARED ${preproc_SRCS})
target_link_libraries(preprocessor ${QT_QTCORE_LIBRARY})
