#ifndef BINDER_H
#define BINDER_H

#include <QObject>
#include <QVariant>

class BinderItem{

public:
    explicit BinderItem(const QList<QVariant> &data, BinderItem *parentItem = 0);
    ~BinderItem();

    void appendChild(BinderItem *child);

    BinderItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    BinderItem *getParentItem();
    void setData(int column, QVariant data);

private:
    QList<BinderItem*> childItems;
    QList<QVariant> itemData;
    BinderItem *parentItem;
};

#endif // BINDER_H
