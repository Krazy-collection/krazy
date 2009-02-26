#include <QtGui/QWidget>

class IncludesTreeModel;
class QAbstractItemModel;
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
    void onTreeTypeChanged(int index);
    void openFile();

  private: // Functions
    QStringList includePaths() const;
    QAbstractItemModel *buildASTModel() const;

  private:
    IncludesTreeModel *m_includeTreeModel;
    Ui::ParseResultWidget *m_ui;
    CPlusPlus::Document *m_selectedDoc;
};
