include_directories(
  ${QT_INCLUDE_DIR}
  ${CPP_SOURCE_DIR}
)

set(cppmodel_SRCS
    nameprettyprinter.cpp
    overview.cpp
    typeprettyprinter.cpp
    macrouse.cpp
    diagnosticmessage.cpp
    document.cpp
    cpppreprocessor.cpp
)

add_library(cppmodel SHARED ${cppmodel_SRCS})

target_link_libraries(cppmodel preprocessor cpp_parser ${QT_QTCORE_LIBRARY})
set_target_properties(cppmodel PROPERTIES DEFINE_SYMBOL CPLUSPLUSMODEL_BUILD_LIB)

if(WIN32)
install(TARGETS preprocessor ${INSTALL_TARGETS_DEFAULT_ARGS})
endif(WIN32)

