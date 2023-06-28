#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "dlglabel.h"
#include "dlgsearchbylabels.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_sqlOperation = new SqliteOperation;
    m_sqlOperation->openDB();
    m_fileBrowser = new FileBrowser(ui->widget);
    m_fileBrowser->setSqliteOperation(m_sqlOperation);

    initLabelList();
    initAllLabelList();

    connect(m_fileBrowser, SIGNAL(sigAddLabels(bool)), this, SLOT(onAddLabel(bool)));
    connect(m_fileBrowser, SIGNAL(sigClickFile(QStringList)), this, SLOT(onClickFile(QStringList)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_fileBrowser;
}

void MainWindow::initLabelList()
{
    m_tvLabels = ui->lvLabels;
    m_modelLabels = new QStandardItemModel;
    m_tvLabels->setModel(m_modelLabels);

    connect(m_modelLabels,SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), this,
            SLOT(onModifyLabel(QModelIndex, QModelIndex, QVector<int>)));
}

void MainWindow::initAllLabelList()
{
    m_lvAllLabels = ui->lvAllLabels;
    m_modelAllLabels = new QStandardItemModel;
    m_lvAllLabels->setModel(m_modelAllLabels);

    m_lvAllLabels->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_vtAllLabels = m_sqlOperation->getAllLabels();
    updateAllLabelList();
}

void MainWindow::updateAllLabelList()
{
    m_modelAllLabels->removeRows(0, m_modelAllLabels->rowCount());
    QStandardItem *item = nullptr;
    for(QVector<QString>::iterator it = m_vtAllLabels.begin(); it != m_vtAllLabels.end(); it++)
    {
        item = new QStandardItem(*it);
        m_modelAllLabels->appendRow(item);
    }
}

void MainWindow::onClickFile(QStringList qLLabels)
{
    m_modelLabels->removeRows(0,m_modelLabels->rowCount());
    for(int i = 0; i < qLLabels.count(); i++)
    {
        QStandardItem* item = new QStandardItem(qLLabels.at(i));
        m_modelLabels->appendRow(item);
    }
}

void MainWindow::on_lvLabels_customContextMenuRequested(const QPoint& pos)
{
    QMenu *cmenu = new QMenu(m_tvLabels);
    QAction *delLabelAction = cmenu->addAction("删除");
    connect(delLabelAction, SIGNAL(triggered(bool)), this, SLOT(onDelLabel()));
    //在当前鼠标位置显示
    cmenu->exec(QCursor::pos());
}

void MainWindow::onAddLabel(bool bIsDir)
{
    QStringList qLLabels;
    QStringList selFilePath;
    m_fileBrowser->getSelFilePath(selFilePath);
    if(selFilePath.count() == 1)
    {
        qLLabels = m_sqlOperation->getLabelsByFile(m_fileBrowser->getCurPath());
    }
    QVector<QString> vtAllLabels = m_sqlOperation->getAllLabels();
    DlgLabel dlg(this);
    dlg.setSqlOperation(m_sqlOperation);
    dlg.setIsDir(bIsDir);
    dlg.setAllLabels(vtAllLabels);
    dlg.setSelLabels(qLLabels);
    if(QDialog::Accepted == dlg.exec())
    {
        QStringList sLabels = dlg.getLabels();
        QStringList sAllLabels = dlg.getAllLabels();

//        // 判断标签是否重复
//        int nRowCount = m_modelLabels->rowCount();
//        for(int i = 0; i < nRowCount; i++)
//        {
//            if(QString::compare(sLabel, m_modelLabels->item(i)->text()) == 0)
//            {
//                qDebug()<<"标签重复！";
//                return;
//            }
//        }

        if(bIsDir)
        {
            bool bIsSetAllFiles = dlg.isSetAllFiles();
        }
        else
        {
            // QTreeView 会默认选中第一个根节点
//            QString sFilePath = m_fileBrowser->getCurPath();
//            if(sFilePath.isEmpty())
//            {
//                qDebug()<<"未选中文件！";
//                return;
//            }

            m_modelAllLabels->removeRows(0, m_modelAllLabels->rowCount());
            QString sLabel;
            QStandardItem* item = nullptr;

            for (int i = 0; i < sAllLabels.count(); i++)
            {
                sLabel = sAllLabels.at(i);
                m_sqlOperation->insertAllLabel(sLabel);

                item = new QStandardItem(sLabel);
                m_modelAllLabels->appendRow(item);

//                if(!m_vtAllLabels.contains(sLabel))
//                {
//                    m_vtAllLabels.push_back(sLabel);
//                    updateAllLabelList();
//                }
            }

            int nFileCount = selFilePath.count();
            for(int nFileNum = 0; nFileNum < nFileCount; nFileNum++)
            {
                // todo 待优化
                m_modelLabels->removeRows(0, m_modelLabels->rowCount());
                m_sqlOperation->clearLabels(selFilePath.at(nFileNum));
                for (int i = 0; i < sLabels.count(); i++)
                {
                    sLabel = sLabels.at(i);
                    m_sqlOperation->insertRecord(selFilePath.at(nFileNum), sLabel);

                    item = new QStandardItem(sLabel);
                    m_modelLabels->appendRow(item);
                }
            }
        }
    }
}

void MainWindow::onDelLabel()
{
    int nRow = m_tvLabels->currentIndex().row();
    QString sLable = m_modelLabels->item(nRow)->text();

    QString sFilePath = m_fileBrowser->getCurPath();
    m_sqlOperation->deleteRecord(sFilePath, sLable);

    m_modelLabels->removeRow(nRow);

    int nIndex = m_vtAllLabels.indexOf(sLable);
    if(nIndex != -1)
    {
        m_vtAllLabels.remove(nIndex);
        updateAllLabelList();
    }
}

void MainWindow::onModifyLabel(QModelIndex index1, QModelIndex index2, QVector<int> vec)
{
    QString sFilePath = m_fileBrowser->getCurPath();

    QModelIndex curLabelIndex = m_tvLabels->currentIndex();
    QString sLabel = m_modelLabels->item(curLabelIndex.row())->text();

    m_sqlOperation->deleteRecord(sFilePath, m_sCurLabel);
    m_sqlOperation->insertRecord(sFilePath, sLabel);
}

void MainWindow::on_lvAllLabels_clicked(const QModelIndex &index)
{
    QString sLabel = m_modelAllLabels->item(index.row())->text();
    m_fileBrowser->filterFile(sLabel);
}

void MainWindow::on_lvLabels_clicked(const QModelIndex &index)
{
    m_sCurLabel = m_modelLabels->item(index.row())->text();
}

void MainWindow::on_actionsearchbylabels_triggered()
{
    DlgSearchByLabels dlg(this);
    dlg.setAllLabels(m_sqlOperation->getAllLabels());
    dlg.initAllLabelList();
    if(QDialog::Accepted == dlg.exec())
    {
        QVector<QString> vtLabels = dlg.getLabels();
        bool bLogicalOperator = dlg.getLogicalOperator();
        if(vtLabels.count() == 0)
            return;
        if(vtLabels.count() == 1)
        {
            m_fileBrowser->filterFile(*vtLabels.begin());
            return;
        }
        m_fileBrowser->filterFile(vtLabels, bLogicalOperator);
    }
}
