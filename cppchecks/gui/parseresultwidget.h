#include <QtGui/QWidget>

namespace Ui
{
  class ParseResultWidget;
}

class ParseResultWidget : public QWidget
{
  public:
    ParseResultWidget();
    ~ParseResultWidget();

  private:
    Ui::ParseResultWidget *m_ui;
};
