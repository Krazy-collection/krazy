set(DPointerCheck_SRCS
  dpointeranalyzer.cpp
  dpointercheckoutputformatter.cpp
  dpointercheck.cpp
  main.cpp 
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/dpointercheck-test.sh
               ${CMAKE_CURRENT_BINARY_DIR}/dpointercheck-test.sh
               COPYONLY
)

add_executable(dpointercheck ${DPointerCheck_SRCS})

target_link_libraries(dpointercheck checkutil cpptools)
if(NOT WIN32)
install(TARGETS dpointercheck DESTINATION ${CMAKE_PREFIX_PATH}/lib/krazy2/krazy-extras/c++)
else(NOT WIN32)
install(TARGETS dpointercheck ${INSTALL_TARGETS_DEFAULT_ARGS})
endif(NOT WIN32)
