#ifndef BINDERMODEL_H
#define BINDERMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>

#include "binderItem.h"

class BinderModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BinderModel(const QString &data, QObject *parent = 0);
    ~BinderModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QString toString();
    QStringList getTitleHierarchy();
    BinderItem* getItem(QModelIndex &index);
    void updateItemParent(BinderItem *item, QString previousParent, QString newParent);
    Qt::DropActions supportedDropActions() const override;

public slots:
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;

private:
    void setupModelData(const QStringList &lines, BinderItem *parent);
    QString parseChild(BinderItem *child, int level);

    BinderItem *rootItem;
    QStringList titleHierarchy;
};

#endif // BINDERMODEL_H
