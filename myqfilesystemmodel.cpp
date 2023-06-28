#include "myqfilesystemmodel.h"
#include <QDebug>
MyQFileSystemModel::MyQFileSystemModel()
{

}

QVariant MyQFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DecorationRole:
        if (section == 0) {
            // ### TODO oh man this is ugly and doesn't even work all the way!
            // it is still 2 pixels off
            QImage pixmap(16, 1, QImage::Format_Mono);
            pixmap.fill(0);
            pixmap.setAlphaChannel(pixmap.createAlphaMask());
            return pixmap;
        }
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignLeft;
    }

    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractItemModel::headerData(section, orientation, role);

    QString returnValue;
    switch (section) {
    case 0: returnValue = tr("名称");
            break;
    case 1: returnValue = tr("大小");
            break;
    case 2: returnValue =tr("类型");
           break;
    case 3: returnValue = tr("修改日期");
            break;
    default: return QVariant();
    }
    return returnValue;
}
