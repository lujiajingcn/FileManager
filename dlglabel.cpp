#include "dlglabel.h"
#include "ui_dlglabel.h"

DlgLabel::DlgLabel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLabel)
{
    ui->setupUi(this);

    // 去掉对话框问号
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    m_lvAllLabels = ui->lvAllLabels;
    m_modelAllLabels = new QStandardItemModel;
    m_lvAllLabels->setModel(m_modelAllLabels);
}

DlgLabel::~DlgLabel()
{
    delete ui;
}

void DlgLabel::setAllLabels(QVector<QString> vtAllLabels)
{
    m_modelAllLabels->removeRows(0, m_modelAllLabels->rowCount());
    QStandardItem *item = nullptr;
    for(QVector<QString>::iterator it = vtAllLabels.begin(); it != vtAllLabels.end(); it++)
    {
        item = new QStandardItem(*it);
        m_modelAllLabels->appendRow(item);
    }
}

void DlgLabel::setLabel(QString sLabel)
{
    ui->leLabel->setText(sLabel);
}

QString DlgLabel::getLabel()
{
    return m_sLabel;
}

void DlgLabel::on_btnOk_clicked()
{
    m_sLabel = ui->leLabel->text();
    accept();
}

void DlgLabel::on_btnCancel_clicked()
{
    reject();
}

void DlgLabel::on_lvAllLabels_clicked(const QModelIndex &index)
{
    QString sLabel = m_modelAllLabels->item(index.row())->text();
    ui->leLabel->setText(sLabel);
}
