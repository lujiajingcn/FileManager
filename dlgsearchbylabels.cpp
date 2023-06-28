#include "dlgsearchbylabels.h"
#include "ui_dlgsearchbylabels.h"

DlgSearchByLabels::DlgSearchByLabels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSearchByLabels)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    m_lvLabels = ui->lvLabels;
    m_modelLabels = new QStandardItemModel;
    m_lvLabels->setModel(m_modelLabels);
    m_lvLabels->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

DlgSearchByLabels::~DlgSearchByLabels()
{
    delete ui;
}

void DlgSearchByLabels::setAllLabels(QVector<QString> qLAllLalels)
{
    m_vtAllLabels = qLAllLalels;
}

void DlgSearchByLabels::initAllLabelList()
{
    m_lvAllLabels = ui->lvAllLabels;
    m_modelAllLabels = new QStandardItemModel;
    m_lvAllLabels->setModel(m_modelAllLabels);

    m_lvAllLabels->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_modelAllLabels->removeRows(0, m_modelAllLabels->rowCount());
    QStandardItem *item = nullptr;
    for(QVector<QString>::iterator it = m_vtAllLabels.begin(); it != m_vtAllLabels.end(); it++)
    {
        item = new QStandardItem(*it);
        m_modelAllLabels->appendRow(item);
    }
}

QVector<QString> DlgSearchByLabels::getLabels()
{
    QVector<QString> vtLabels;
    int nRowCount = m_modelLabels->rowCount();
    for(int i = 0; i < nRowCount; i++)
    {
        QString sLabel = m_modelLabels->item(i)->text();
        vtLabels.push_back(sLabel);
    }
    return vtLabels;
}

bool DlgSearchByLabels::getLogicalOperator()
{
    return ui->rbAnd->isChecked();
}

void DlgSearchByLabels::on_btnAdd_clicked()
{
    addLabel();
}

void DlgSearchByLabels::on_btnDelete_clicked()
{
    deleteLabel();
}

void DlgSearchByLabels::on_lvAllLabels_doubleClicked(const QModelIndex &index)
{
    addLabel();
}

void DlgSearchByLabels::on_lvLabels_doubleClicked(const QModelIndex &index)
{
    deleteLabel();
}

void DlgSearchByLabels::on_btnSearch_clicked()
{
    accept();
}

void DlgSearchByLabels::on_btnCancel_clicked()
{
    reject();
}

void DlgSearchByLabels::addLabel()
{
    int nCurRow = m_lvAllLabels->currentIndex().row();
    if(nCurRow == -1)
        return;
    QString sLabel = m_modelAllLabels->item(nCurRow)->text();
    int nRowCount = m_modelLabels->rowCount();
    for(int i = 0; i < nRowCount; i++)
    {
        QString sTemp = m_modelLabels->item(i)->text();
        if(QString::compare(sLabel, sTemp) == 0)
        {
            return;
        }
    }
    QStandardItem *item = new QStandardItem(sLabel);
    m_modelLabels->appendRow(item);
}

void DlgSearchByLabels::deleteLabel()
{
    int nCurRow = m_lvLabels->currentIndex().row();
    if(nCurRow == -1)
        return;
    m_modelLabels->removeRow(nCurRow);
}
