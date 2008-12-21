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
