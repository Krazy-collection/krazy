set(checkutil_SRCS
  checkengine.cpp
  check.cpp
)

add_library(checkutil SHARED ${checkutil_SRCS})

target_link_libraries(checkutil
    ${QT_QTCORE_LIBRARY}
    # For these two you need to install KDevelop
    c++parser
)
set_target_properties(checkutil PROPERTIES VERSION 1 SOVERSION 1.0)
install(TARGETS checkutil DESTINATION ${CMAKE_PREFIX_PATH}/lib)
