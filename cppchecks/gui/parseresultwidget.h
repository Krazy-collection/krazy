#include <QtGui/QWidget>

class QModelIndex;

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
    void onClicked(QModelIndex const &);
    void openFile();

  private: // Functions
    QStringList includePaths() const;

  private:
    Ui::ParseResultWidget *m_ui;
};
