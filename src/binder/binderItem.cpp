#include "binderItem.h"

BinderItem::BinderItem(const QList<QVariant> &data, BinderItem *parent)
{
    parentItem = parent;
    itemData = data;
}

BinderItem::~BinderItem()
{
    qDeleteAll(childItems);
}

void BinderItem::appendChild(BinderItem *item)
{
    childItems.append(item);
}

BinderItem *BinderItem::child(int row)
{
    return childItems.value(row);
}

int BinderItem::childCount() const
{
    return childItems.count();
}

int BinderItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<BinderItem*>(this));

    return 0;
}

int BinderItem::columnCount() const
{
    return itemData.count();
}

QVariant BinderItem::data(int column) const
{
    return itemData.value(column);
}

BinderItem *BinderItem::getParentItem()
{
    return parentItem;
}
