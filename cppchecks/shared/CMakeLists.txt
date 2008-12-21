set(shared_SRCS
  checkengine.cpp
  check.cpp
)

add_library(shared SHARED ${shared_SRCS})

target_link_libraries(shared
    ${QT_QTCORE_LIBRARY}
    # For these two you need to install KDevelop
    ${KDE4_LIB_DIR}/libkdev4cppparser.so
    ${KDE4_LIB_DIR}/libkdev4cpprpp.so
)
set_target_properties(shared PROPERTIES VERSION 1 SOVERSION 1.0)
install(TARGETS shared DESTINATION ${CMAKE_PREFIX_PATH}/lib)
