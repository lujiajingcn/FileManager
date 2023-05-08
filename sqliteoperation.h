#ifndef SQLITEOPERATION_H
#define SQLITEOPERATION_H

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQueryModel>

class SqliteOperation
{
public:
    SqliteOperation();
    ~SqliteOperation();

    void openDB();
    void createTable();
    void clearLabels(const QString &sFilePath);
    void insertRecord(const QString &sFilePath, QString labels);
    void deleteRecord(const QString &sFilePath, const QString &sLabel);
    void update(QString sOldLable, QString sNewLable, QString sFilePath);
    QStringList getLabelsByFile(const QString &sFilePath);
    QVector<QString> getAllLabels();

    bool addLabel(QString sLabel);
    bool delLabel(QString sLabel);
    bool modLabel(QString sOldLabel, QString sNewLabel);

public:
    QSqlDatabase    m_sqlDB;
};

#endif // SQLITEOPERATION_H
