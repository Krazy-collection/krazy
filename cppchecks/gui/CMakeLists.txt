include_directories(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

qt4_wrap_ui(APP_UI_HDRS parseresultwidget.ui)

set(APP_SRCS
  parseresultwidget.cc
  main.cc
)

add_executable(parse_result_analyzer ${APP_SRCS} ${APP_UI_HDRS})
target_link_libraries(parse_result_analyzer
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)
