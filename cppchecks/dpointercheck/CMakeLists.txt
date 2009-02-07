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
install(TARGETS dpointercheck DESTINATION ${CMAKE_PREFIX_PATH}/lib/krazy2/krazy-extras/c++)
