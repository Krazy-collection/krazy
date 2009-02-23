include_directories(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CPP_PARSER_SOURCE_DIR}
  ${CPP_PREPROCESSOR_SOURCE_DIR}
)

set(APP_SRCS
  includestreemodel.cc
  parseresultwidget.cc
  main.cc
)

set(APP_MOC_HDRS
  parseresultwidget.h
)

qt4_wrap_ui(APP_UI_HDRS parseresultwidget.ui)
qt4_wrap_cpp(APP_MOC_SRCS ${APP_MOC_HDRS})

add_executable(parse_result_analyzer ${APP_SRCS} ${APP_MOC_SRCS} ${APP_UI_HDRS})
target_link_libraries(parse_result_analyzer
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  preprocessor
)
