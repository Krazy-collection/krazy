#include <QtGui/QWidget>

class QModelIndex;

namespace CPlusPlus
{
  class Document;
}

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
    void onStateChanged(int);
    void openFile();

  private: // Functions
    QStringList includePaths() const;

  private:
    Ui::ParseResultWidget *m_ui;
    CPlusPlus::Document *m_selectedDoc;
};
