include_directories(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_SOURCE_DIR} STATUS)

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

add_library(preproc SHARED ${preproc_SRCS})
 
