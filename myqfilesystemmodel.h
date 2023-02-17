#ifndef MYQFILESYSTEMMODEL_H
#define MYQFILESYSTEMMODEL_H

#include <QFileSystemModel>

class MyQFileSystemModel :public QFileSystemModel
{
public:
    MyQFileSystemModel();

    // 将表头的字段名由默认的Name、Size、Type、Date Modified，改为名称、大小、类型、修改日期
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

};

#endif // MYQFILESYSTEMMODEL_H
