#include "parseresultwidget.h"

#include "ui_parseresultwidget.h"

ParseResultWidget::ParseResultWidget()
  : m_ui(new Ui::ParseResultWidget())
{
  m_ui->setupUi(this);
}

ParseResultWidget::~ParseResultWidget()
{
  delete m_ui;
}
