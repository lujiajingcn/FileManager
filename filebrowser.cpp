#include "filebrowser.h"
#include "ui_filebrowser.h"
#include <QMenu>
#include <QDebug>

FileBrowser::FileBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBrowser)
{
    ui->setupUi(this);

    createToolButtons();
    createMenuActions();

    initNavigation();
    initFileList();
    initFilteredFileList();

    setWindowStyleSheet();
}

FileBrowser::~FileBrowser()
{
    delete ui;
}

void FileBrowser::createToolButtons()
{
    ui->backButton->setIcon(style()->standardIcon(QStyle::SP_ArrowBack, nullptr, this));
    ui->backButton->setAutoRaise(true);
    QObject::connect(ui->backButton, SIGNAL(clicked()), this, SLOT(navigateBackward()));

    ui->forwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward, nullptr, this));
    ui->forwardButton->setAutoRaise(true);
    QObject::connect(ui->forwardButton, SIGNAL(clicked()), this, SLOT(navigateForward()));

    ui->toParentButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogToParent, nullptr, this));
    ui->toParentButton->setAutoRaise(true);
    QObject::connect(ui->toParentButton, SIGNAL(clicked()), this, SLOT(navigateToParent()));
}

void FileBrowser::createMenuActions()
{
    m_addLabelAction = new QAction("设置标签", this);
    connect(m_addLabelAction, SIGNAL(triggered()), this, SLOT(onAddLabel()));

    m_gotoFilePath = new QAction("打开文件所在位置", this);
}

void FileBrowser::onAddLabel()
{
    QModelIndex index = m_tvFiles->currentIndex();
    bool bIsDir = m_modelFiles->isDir(index);
    qDebug()<<bIsDir;

    emit sigAddLabels(bIsDir);
}

void FileBrowser::initNavigation()
{
    m_tvNavigation = ui->tvNavigation;
    m_modelNavigation = new QFileSystemModel;
    m_tvNavigation->setModel(m_modelNavigation);

    m_modelNavigation->setRootPath(QDir::rootPath());

    // QFileSystemModel 默认有四列信息：Name、Size、Type、Date Modified，
    // 我们只需要Name，其余三列隐藏。
    m_tvNavigation->setColumnHidden(1, true);
    m_tvNavigation->setColumnHidden(2, true);
    m_tvNavigation->setColumnHidden(3, true);

    // 只显示文件夹
    m_modelNavigation->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    // 文件夹根据名称升序排序
    m_tvNavigation->header()->setSortIndicator(0,Qt::AscendingOrder);
    m_tvNavigation->setSortingEnabled(true);
    m_tvNavigation->sortByColumn(0, Qt::AscendingOrder);
}

void FileBrowser::initFileList()
{
    m_tvFiles = ui->tvFiles;
    m_tvFiles->setSelectionMode(QAbstractItemView::ExtendedSelection); // 设置可以选中多行
    m_modelFiles = new MyQFileSystemModel;
    m_tvFiles->setModel(m_modelFiles);

    m_modelFiles->setRootPath("My Computer");
    m_tvFiles->setRootIndex(m_modelFiles->index("My Computer"));
    m_tvFiles->setSortingEnabled(true);
    m_tvFiles->header()->setSortIndicator(0, Qt::AscendingOrder);

    // 设置不能展开节点，以使得QTreeView表现的像QListView一样。
    m_tvFiles->setItemsExpandable(false);

    // 设置表头
    // 拷贝自Qt源码的Src\qtbase\src\widgets\dialogs\qfiledialg.cpp
    QHeaderView *treeHeader = m_tvFiles->header();
    QFontMetrics fm(m_tvFiles->font());
    treeHeader->resizeSection(0, fm.horizontalAdvance(QLatin1String("wwwwwwwwwwwwwwwwwwwwwwwwww")));
    treeHeader->resizeSection(1, fm.horizontalAdvance(QLatin1String("128.88 GB")));
    treeHeader->resizeSection(2, fm.horizontalAdvance(QLatin1String("mp3Folder")));
    treeHeader->resizeSection(3, fm.horizontalAdvance(QLatin1String("10/29/81 02:02PM")));
    treeHeader->setContextMenuPolicy(Qt::ActionsContextMenu);

    m_sCurPath = "My Computer";
}

void FileBrowser::initFilteredFileList()
{
    m_tvFilteredFiles = ui->tvFilteredFiles;
    m_modelFilteredFiles = new QStandardItemModel;
    m_tvFilteredFiles->setModel(m_modelFilteredFiles);

    connect(m_gotoFilePath, SIGNAL(triggered(bool)), this, SLOT(onGotoFilePath()));
}

void FileBrowser::curDirChanged(QString sCurDir)
{
    ui->lookInCombo->setEditText(sCurDir);
    m_skBackward.push_back(m_sCurPath);
    m_skForward.clear();
    m_sCurPath = sCurDir;
}

void FileBrowser::setWindowStyleSheet()
{
    QList<int> sizes;
    // 在保持比例的情况下，绝对值要尽量大
    sizes << 10000 << 40000;
    ui->splitter->setSizes(sizes);

    // 设置初始窗口大小
    resize(1500,800);

    // 初始显示文件列表
    ui->stackedWidget->setCurrentWidget(ui->pageFiles);

    QVBoxLayout *lyt = new QVBoxLayout;
    lyt->addWidget(this);
    lyt->setMargin(0);
    static_cast<QWidget*>(parent())->setLayout(lyt);
}

bool FileBrowser::event(QEvent *event)
{
    if((event->type() == QEvent::Type::Paint))
    {
        updateToolButtons();
    }

    return QWidget::event(event);
}

void FileBrowser::updateToolButtons()
{
    if(m_skBackward.count() == 0)
        ui->backButton->setEnabled(false);
    else
        ui->backButton->setEnabled(true);

    if(m_skForward.count() == 0)
        ui->forwardButton->setEnabled(false);
    else
        ui->forwardButton->setEnabled(true);

    QModelIndex rootIndex = m_tvFiles->rootIndex();
    QString sFilePath = m_modelFiles->filePath(rootIndex);
    if(sFilePath.isEmpty())
        ui->toParentButton->setEnabled(false);
    else
        ui->toParentButton->setEnabled(true);
}

void FileBrowser::getFilesInCurDir(QVector<QString> &vtFilePath)
{

}

void FileBrowser::navigateBackward()
{
    QString sDir = m_skBackward.pop();
    m_skForward.push_back(m_sCurPath);
    m_sCurPath = sDir;
    m_tvFiles->setRootIndex(m_modelFiles->index(sDir));
    ui->lookInCombo->setEditText(sDir);
}

void FileBrowser::navigateForward()
{
    QString sDir = m_skForward.pop();
    m_skBackward.push_back(m_sCurPath);
    m_sCurPath = sDir;
    m_tvFiles->setRootIndex(m_modelFiles->index(sDir));
    ui->lookInCombo->setEditText(sDir);
}

void FileBrowser::navigateToParent()
{
    QModelIndex rootIndex = m_tvFiles->rootIndex();
    QString sFilePath = m_modelFiles->filePath(rootIndex);
    QDir dir(sFilePath);
    QString newDirectory;
    if (dir.isRoot()) {
        newDirectory = m_modelFiles->myComputer().toString();
    }
    else
    {
        dir.cdUp();
        newDirectory = dir.absolutePath();
    }
    m_tvFiles->setRootIndex(m_modelFiles->index(newDirectory));

    curDirChanged(newDirectory);
}

void FileBrowser::onGotoFilePath()
{
    int nCurRow = ui->tvFilteredFiles->currentIndex().row();
    QString sFilePath = m_modelFilteredFiles->item(nCurRow)->text();
    QString sFileDir = sFilePath.left(sFilePath.lastIndexOf("/"));
    m_tvFiles->setRootIndex(m_modelFiles->index(sFileDir));
    ui->stackedWidget->setCurrentWidget(ui->pageFiles);
    m_tvFiles->setCurrentIndex(m_modelFiles->index(sFilePath));
}

void FileBrowser::on_tvNavigation_clicked(const QModelIndex &index)
{
    QString path = index.data(QFileSystemModel::FilePathRole).toString();
    m_tvFiles->setRootIndex(m_modelFiles->index(path));
    curDirChanged(path);
    ui->stackedWidget->setCurrentWidget(ui->pageFiles);
}

QString FileBrowser::getCurPath()
{
    QString sCurPath;
    if(ui->stackedWidget->currentWidget() == ui->pageFiles)
    {
        QModelIndex curIndex = m_tvFiles->currentIndex();
        if(!curIndex.isValid())
            return "";
        sCurPath = m_modelFiles->filePath(curIndex);
    }
    else if (ui->stackedWidget->currentWidget() == ui->pageFilteredFiles)
    {

    }
    return sCurPath;
}

void FileBrowser::getSelFilePath(QStringList &selFilePath)
{
    QModelIndexList selected = m_tvFiles->selectionModel()->selectedRows();
    QString sSelect;
    for (QModelIndexList::const_iterator cit = selected.begin(); cit != selected.end(); ++cit)
    {
        selFilePath << m_modelFiles->filePath(*cit);
    }
}

void FileBrowser::setSqliteOperation(SqliteOperation *sqlilteOperation)
{
    m_sqlOperation = sqlilteOperation;
}

void FileBrowser::filterFile(QString sLabel)
{
    QString sSql = QString("select filepath from filewithlabels where label='%1'").arg(sLabel);
    updateFilterFile(sSql);
}

void FileBrowser::filterFile(QVector<QString> vtLabels, bool isAnd)
{
    QString sSql;
    if(isAnd)
    {
        QString sSelect,sUnionSelect;
        for (QVector<QString>::iterator it = vtLabels.begin(); it != vtLabels.end(); it++)
        {
            sSelect = QString("select * from filewithlabels where label='%1'").arg(*it);
            if(sUnionSelect.isEmpty())
            {
                sUnionSelect = sSelect;
            }
            else
            {
                sUnionSelect = QString("%1 union %2").arg(sUnionSelect).arg(sSelect);
            }
        }
        sSql = QString("select filepath from (%1) group by(filepath) having count(filepath)=%2").arg(sUnionSelect).arg(vtLabels.count());
    }
    else
    {
        QString sWhere, sCondition;
        for (QVector<QString>::iterator it = vtLabels.begin(); it != vtLabels.end(); it++)
        {
            sCondition = QString("label='%1'").arg(*it);
            if(sWhere.isEmpty())
            {
                sWhere = sCondition;
            }
            else
            {
                sWhere = QString("%1 or %2").arg(sWhere).arg(sCondition);
            }
        }
        sSql = QString("select filepath from filewithlabels where %1 group by(filepath)").arg(sWhere);
    }
    updateFilterFile(sSql);
}

void FileBrowser::updateFilterFile(QString sSql)
{
    ui->stackedWidget->setCurrentWidget(ui->pageFilteredFiles);

    QSqlQuery query(m_sqlOperation->m_sqlDB);
    query.exec(sSql);
    QStringList qLFilePaths;
    while(query.next())
    {
        QString sFilePath=query.value(0).toString();
        qLFilePaths<<sFilePath;
    }

    m_modelFilteredFiles->removeRows(0, m_modelFilteredFiles->rowCount());
    for(int i = 0; i < qLFilePaths.count(); i++)
    {
        QStandardItem* item = new QStandardItem(qLFilePaths.at(i));
        m_modelFilteredFiles->appendRow(item);
    }
}

void FileBrowser::on_tvFiles_clicked(const QModelIndex &index)
{
    QStringList qLLabels;
    QModelIndexList selected = m_tvFiles->selectionModel()->selectedRows();
    if(selected.count() > 1)
    {

    }
    else
    {
        QString sFilePath = m_modelFiles->filePath(index);
        qLLabels = m_sqlOperation->getLabelsByFile(sFilePath);
    }

    emit sigClickFile(qLLabels);
}

void FileBrowser::on_tvFiles_doubleClicked(const QModelIndex &index)
{
    QFileInfo fileInfo = m_modelFiles->fileInfo(index);
    if(fileInfo.isDir())
    {
        // 如果双击的是文件夹，则将该文件夹内的列表显示。
        QString path = index.data(QFileSystemModel::FilePathRole).toString();
        m_tvFiles->setRootIndex(m_modelFiles->index(path));
        curDirChanged(path);
    }
    else if(fileInfo.isFile())
    {
        // todo
        // 如果双击的是文件,则直接打开文件。
    }
}

void FileBrowser::on_tvFiles_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(m_tvFiles);
    menu.addAction(m_addLabelAction);
    menu.exec(QCursor::pos());
}

void FileBrowser::on_tvFilteredFiles_customContextMenuRequested(const QPoint &position)
{
    QMenu menu(ui->tvFilteredFiles);
    menu.addAction(m_gotoFilePath);
    menu.exec(QCursor::pos());
}

void FileBrowser::on_tvFilteredFiles_clicked(const QModelIndex &index)
{
    QString sFilePath = m_modelFilteredFiles->item(index.row())->text();
    QStringList qLLabels = m_sqlOperation->getLabelsByFile(sFilePath);
    emit sigClickFile(qLLabels);
}
