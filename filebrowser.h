#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QWidget>
#include <QTreeView>
#include <QListView>
#include <QFileSystemModel>
#include <QStack>
#include <QAction>
#include <QStandardItemModel>

#include "myqfilesystemmodel.h"
#include "sqliteoperation.h"

namespace Ui {
class FileBrowser;
}

class FileBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowser(QWidget *parent = nullptr);
    ~FileBrowser();

    void createToolButtons();
    void createMenuActions();

    // 初始化左侧导航栏
    void initNavigation();

    // 初始化文件列表
    void initFileList();

    // 初始化根据标签筛选出的文件列表
    void initFilteredFileList();

    // 设置样式，大部分样式已经在filebrowser.ui中设置完成，在此进行少量样式设置
    void setWindowStyleSheet();

    void curDirChanged(QString sCurDir);

    // 获取当前选中文件的路径
    // 如果返回值为空，表示未选中文件
    QString getCurPath();

    void setSqliteOperation(SqliteOperation sqlilteOperation);

    void filterFile(QString sLabel);
    void filterFile(QVector<QString> vtLabels, bool isAnd);
    void updateFilterFile(QString sSql);

    bool event(QEvent *event);
    void updateToolButtons();

signals:
    void sigAddLabels(bool bIsDir);
    void sigClickFile(QStringList sLabels);

public slots:

    // 后退
    void navigateBackward();

    // 前进
    void navigateForward();

    // 返回父级文件夹
    void navigateToParent();

    void onGotoFilePath();

    // 点击左侧导航栏中节点，将该节点下的文件夹及文件显示在右侧文件列表
    void on_tvNavigation_clicked(const QModelIndex &index);

    // 点击文件列表中的文件，将该文件的标签显示在标签列表
    void on_tvFiles_clicked(const QModelIndex &index);

    // 双击文件列表中的文件夹，进入该文件夹
    void on_tvFiles_doubleClicked(const QModelIndex &index);

    // 文件列表的右键菜单
    void on_tvFiles_customContextMenuRequested(const QPoint &pos);

    // 过滤后的文件列表的右键菜单
    void on_tvFilteredFiles_customContextMenuRequested(const QPoint &pos);

    void onAddLabel();

private slots:
    void on_tvFilteredFiles_clicked(const QModelIndex &index);

private:
    SqliteOperation         m_sqlOperation;         // 操作sqlite数据库

    QTreeView               *m_tvNavigation;        // 左侧导航栏
    QFileSystemModel        *m_modelNavigation;

    QTreeView               *m_tvFiles;             // 文件列表，由于QListView不能显示表头，故用QTreeView实现列表功能。
    MyQFileSystemModel      *m_modelFiles;

    QTreeView               *m_tvFilteredFiles;     // 根据标签筛选出的的文件列表
    QStandardItemModel      *m_modelFilteredFiles;

    QAction                 *m_addLabelAction;      // 文件列表tvFiles的右键菜单操作

    QAction                 *m_gotoFilePath;        // 筛选过的文件列表的右键菜单操作

    QStack<QString>         m_skBackward;           // 访问过的路径，用于后退功能
    QStack<QString>         m_skForward;            // 访问过的路径，用于前进功能
    QString                 m_sCurPath;             // 当前正在访问的路径

    Ui::FileBrowser *ui;
};

#endif // FILEBROWSER_H
