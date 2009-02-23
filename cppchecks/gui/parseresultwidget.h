#include <QtGui/QWidget>

namespace Ui
{
  class ParseResultWidget;
}

class ParseResultWidget : public QWidget
{
  Q_OBJECT

  public:
    ParseResultWidget();
    ~ParseResultWidget();

  public slots:
    void openFile();

  private:
    Ui::ParseResultWidget *m_ui;
};
