#ifndef DLGLABEL_H
#define DLGLABEL_H

#include <QDialog>
#include <QListView>
#include <QStandardItemModel>

namespace Ui {
class DlgLabel;
}

class DlgLabel : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLabel(QWidget *parent = nullptr);
    ~DlgLabel();

    void setIsDir(bool bIsDir);
    void setAllLabels(QVector<QString> vtAllLabels);

    void setLabel(QString sLabel);
    QString getLabel();

    // 设置当前文件夹和文件夹中的全部文件，还是只是设置当前文件夹
    bool isSetAllFiles();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_lvAllLabels_clicked(const QModelIndex &index);

private:
    QString                 m_sLabel;

    QListView               *m_lvAllLabels;         // 所有标签列表
    QStandardItemModel      *m_modelAllLabels;

    Ui::DlgLabel            *ui;
};

#endif // DLGLABEL_H
