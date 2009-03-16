#include <QtGui/QWidget>

#include <cppmodel/document.h>
#include <parser/Scope.h>

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

  protected slots:
    void exportAST();
    void exportScope();
    void onASTItemClicked(QModelIndex const &);
    void onIncludeClicked(QModelIndex const &);
    void onStateChanged(int);
    void onTreeTypeChanged(int index);
    void openFile();

  private: // Functions
    QAbstractItemModel *buildScopeModel();
    QStringList includePaths() const;

  private:
    IncludesTreeModel                 *m_includeTreeModel;
    CPlusPlus::CppModel::Document::Ptr m_rootDoc;
    CPlusPlus::CppModel::Document::Ptr m_selectedDoc;
    Ui::ParseResultWidget             *m_ui;
	CPlusPlus::Scope                  *m_globals;
};
