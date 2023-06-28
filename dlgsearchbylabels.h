#ifndef DLGSEARCHBYLABELS_H
#define DLGSEARCHBYLABELS_H

#include <QDialog>
#include <QListView>
#include <QStandardItemModel>

namespace Ui {
class DlgSearchByLabels;
}

class DlgSearchByLabels : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSearchByLabels(QWidget *parent = nullptr);
    ~DlgSearchByLabels();

    void setAllLabels(QVector<QString> qLAllLalels);
    void initAllLabelList();
    QVector<QString> getLabels();
    bool getLogicalOperator();

private slots:
    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_lvAllLabels_doubleClicked(const QModelIndex &index);

    void on_lvLabels_doubleClicked(const QModelIndex &index);

    void on_btnSearch_clicked();

    void on_btnCancel_clicked();

private:
    void addLabel();
    void deleteLabel();

private:
    QListView               *m_lvLabels;
    QStandardItemModel      *m_modelLabels;

    QListView               *m_lvAllLabels;
    QStandardItemModel      *m_modelAllLabels;

    QVector<QString>        m_vtAllLabels;
    QVector<QString>        m_vtLabels;

    bool                    m_bIsAnd;

    Ui::DlgSearchByLabels *ui;
};

#endif // DLGSEARCHBYLABELS_H
