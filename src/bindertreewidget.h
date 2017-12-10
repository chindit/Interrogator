#ifndef BINDERBUILDER_H
#define BINDERBUILDER_H

#include <QObject>
#include <QTreeWidget>

#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>

#include <QFile>
#include <QTextStream>

#include "tools/constants.h"

class BinderTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit BinderTreeWidget(QWidget *parent = nullptr);
    void saveXML();
    void readXML();

signals:

public slots:

private:
    void enableDragAndDrop();
    QDomElement parseItem(QTreeWidgetItem *item, QDomDocument root);
    QTreeWidgetItem* createItem(QDomElement node);
};

#endif // BINDERBUILDER_H
