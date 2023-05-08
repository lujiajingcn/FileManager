#include "sqliteoperation.h"

#include <QDebug>

SqliteOperation::SqliteOperation()
{

}

SqliteOperation::~SqliteOperation()
{
    m_sqlDB.close();
}

void SqliteOperation::openDB()
{
    m_sqlDB = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDB.setDatabaseName("FileManager.db");
    if(!m_sqlDB.open())
    {
        qDebug() << "open database fails!" << m_sqlDB.lastError();
    }

    // 在sql语句中判断表是否存在。
    // 如果没有表filewithlabels和alllabels，则创建这两个表
    createTable();
}

void SqliteOperation::createTable()
{
    QSqlQuery query(m_sqlDB);
    QString sSql = R"(
                      CREATE TABLE IF NOT EXISTS  filewithlabels (
                      id   INTEGER   PRIMARY KEY AUTOINCREMENT NOT NULL,
                      filepath CHAR (260) NOT NULL,
                      label   TEXT (1024) NOT NULL)
                      )";
    if(!query.exec(sSql))
    {
        qDebug()<<"create table error"<<query.lastError();
    }

    sSql = R"(
              CREATE TABLE IF NOT EXISTS  alllabels (
              label   TEXT (1024) UNIQUE NOT NULL)
              )";
    if(!query.exec(sSql))
    {
        qDebug()<<"create table error"<<query.lastError();
    }
}

void SqliteOperation::clearLabels(const QString &sFilePath)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("DELETE FROM filewithlabels WHERE filepath='%1'")
                   .arg(sFilePath);
    bool bRet = query.exec(sSql);
    if(!bRet)
    {
        qDebug()<<"删除数据失败！";
    }
}

void SqliteOperation::insertRecord(const QString &path, QString labels)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("INSERT INTO filewithlabels (filepath,label) VALUES('%1','%2')")
                   .arg(path).arg(labels);
//    QString sSql = QString("INSERT INTO filewithlabels(filepath, label)"
//                           "SELECT '%1', '%2' FROM dual WHERE not exists (select * from filewithlabels where filewithlabels.filepath = '%3' and filewithlabels.label = '%4')")
//                   .arg(path).arg(labels).arg(path).arg(labels);
//    QString sSql = QString("INSERT INTO filewithlabels(filepath, label) "
//                           "SELECT '%1', '%2' FROM filewithlabels WHERE not exists (select * from filewithlabels where filewithlabels.filepath = '%3' and filewithlabels.label = '%4')")
//                   .arg(path).arg(labels).arg(path).arg(labels);
    bool bRet = query.exec(sSql);
    if(!bRet)
    {
        qDebug()<<"插入数据失败！";
    }
}

void SqliteOperation::insertAllLabel(const QString sLabel)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("INSERT INTO alllabels (label) VALUES('%1')")
                   .arg(sLabel);
    bool bRet = query.exec(sSql);
    if(!bRet)
    {
//        qDebug()<<"插入数据失败！";
    }
}

void SqliteOperation::deleteRecord(const QString &sFilePath, const QString &sLabel)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("DELETE FROM filewithlabels WHERE filepath='%1' and label='%2'")
                   .arg(sFilePath).arg(sLabel);
    bool bRet = query.exec(sSql);
    if(!bRet)
    {
        qDebug()<<"删除数据失败！";
    }

    sSql = QString("DELETE FROM alllabels WHERE label='%2'")
           .arg(sLabel);
    bRet = query.exec(sSql);
    if(!bRet)
    {
        qDebug()<<"删除数据失败！";
    }
}

void SqliteOperation::update(QString sOldLable, QString sNewLable, QString sFilePath)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("update filewithlabels set label='%1' where filepath='%2' and label='%3'")
                            .arg(sNewLable).arg(sFilePath).arg(sOldLable);
    query.exec(sSql);
}

QStringList SqliteOperation::getLabelsByFile(const QString &sFilePath)
{
    QSqlQuery query(m_sqlDB);
    query.exec(QString("SELECT label FROM filewithlabels WHERE filepath='%1'")
               .arg(sFilePath));

    QStringList qLLables;
    QString sLabel;
    while(query.next())
    {
        sLabel = query.value(0).toString();
        qLLables << sLabel;
    }
    return qLLables;
}

QVector<QString> SqliteOperation::getAllLabels()
{
    QSqlQuery query(m_sqlDB);
    query.exec(QString("SELECT label FROM alllabels"));

    QVector<QString> qLAllLables;
    QString sLabel;
    while(query.next())
    {
        sLabel = query.value(0).toString();
        qLAllLables << sLabel;
    }
    return qLAllLables;
}

bool SqliteOperation::addLabel(QString sLabel)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("INSERT INTO alllabels(label) VALUES('%1')")
           .arg(sLabel);
    int bRet = query.exec(sSql);
    if(!bRet)
    {
        return false;
    }
    return true;
}

bool SqliteOperation::delLabel(QString sLabel)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("DELETE FROM alllabels WHERE label='%1'")
           .arg(sLabel);
    int bRet = query.exec(sSql);
    if(!bRet)
    {
        return false;
    }
    return true;
}

bool SqliteOperation::modLabel(QString sOldLabel, QString sNewLabel)
{
    QSqlQuery query(m_sqlDB);
    QString sSql = QString("update alllabels set label='%1' WHERE label='%2'")
           .arg(sNewLabel).arg(sOldLabel);
    int bRet = query.exec(sSql);
    if(!bRet)
    {
        return false;
    }
    return true;
}
