#include "bindermodel.h"

BinderModel::BinderModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << tr("Titre") << tr("Description");
    rootItem = new BinderItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

BinderModel::~BinderModel()
{
    delete rootItem;
}

int BinderModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<BinderItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant BinderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    BinderItem *item = static_cast<BinderItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags BinderModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant BinderModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex BinderModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    BinderItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<BinderItem*>(parent.internalPointer());

    BinderItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex BinderModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    BinderItem *childItem = static_cast<BinderItem*>(index.internalPointer());
    BinderItem *parentItem = childItem->getParentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int BinderModel::rowCount(const QModelIndex &parent) const
{
    BinderItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<BinderItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void BinderModel::setupModelData(const QStringList &lines, BinderItem *parent)
{
    QList<BinderItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new BinderItem(columnData, parents.last()));
        }

        ++number;
    }
}

QString BinderModel::toString()
{
    QString output = QString();
    int level = 0;
    for (int i=0; i < this->rootItem->childCount(); i++) {
        output.append(this->parseChild(this->rootItem->child(i), level));
    }

    return output;
}

QString BinderModel::parseChild(BinderItem *child, int level)
{
    QString result = QString();
    if (child->childCount() > 0) {
        for (int i=0; i < child->childCount(); i++) {
            result.append(this->parseChild(child->child(i), level+1));
        }
    }

    QString temp = QString();
    temp.append(QString(' ').repeated(level*4)).append(child->data(0).toString()).append("\t").append(child->data(1).toString()).append("\n");

    return result.prepend(temp);
}
