set(CHECKUTIL_SOURCE_DIR ${CMAKE_SOURCE_DIR}/checkutil)

include_directories(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CPP_SOURCE_DIR}
  ${CHECKUTIL_SOURCE_DIR}
)

add_subdirectory(dpointercheck)
