#include <QtGui/QWidget>

#include <CppDocument.h>

class IncludesTreeModel;
class QAbstractItemModel;
class QModelIndex;

namespace CPlusPlus
{
  class Scope;
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
    void exportAST();
    void onASTItemClicked(QModelIndex const &);
    void onIncludeClicked(QModelIndex const &);
    void onStateChanged(int);
    void onTreeTypeChanged(int index);
    void openFile();

  private: // Functions
    QStringList includePaths() const;
    QAbstractItemModel *buildASTModel() const;
    QAbstractItemModel *buildScopeModel();

  private:
    CPlusPlus::Scope        *m_globals;
    IncludesTreeModel       *m_includeTreeModel;
    CPlusPlus::Document::Ptr m_rootDoc;
    CPlusPlus::Document     *m_selectedDoc;
    Ui::ParseResultWidget   *m_ui;
};
