#include <QtGui/QApplication>

#include "parseresultwidget.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  ParseResultWidget widget;
  widget.show();

  return app.exec();
}
