#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QListView>
#include <QStandardItemModel>
#include <QFileDialog>

#include "filebrowser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 初始化选中文件的标签列表
    void initLabelList();

    // 初始化全部标签列表
    void initAllLabelList();

    // 刷新全部标签列表
    void updateAllLabelList();

private slots:

    // 点击文件列表，展示点击的文件具有的标签
    void onClickFile(QStringList qLLabels);

    // 标签列表的右键菜单
    void on_lvLabels_customContextMenuRequested(const QPoint &pos);

    // 添加标签
    void onAddLabel(bool bIsDir = false);

    // 删除标签
    void onDelLabel();

    // 修改标签
    void onModifyLabel(QModelIndex index1, QModelIndex index2, QVector<int> vec);

    // 点击全部标签列表
    void on_lvAllLabels_clicked(const QModelIndex &index);

    // 更新当前标签
    void on_lvLabels_clicked(const QModelIndex &index);

    void on_actionsearchbylabels_triggered();

private:
    SqliteOperation         m_sqlOperation;         // 操作sqlite数据库

    FileBrowser             *m_fileBrowser;         // 文件操作

    QListView               *m_tvLabels;            // 选中文件的标签列表
    QStandardItemModel      *m_modelLabels;

    QListView               *m_lvAllLabels;         // 所有标签列表
    QStandardItemModel      *m_modelAllLabels;

    QVector<QString>        m_vtAllLabels;          // 全部标签

    QString                 m_sCurLabel;            // 在lvLabels中当前选中的标签

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
