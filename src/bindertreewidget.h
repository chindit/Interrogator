#ifndef BINDERBUILDER_H
#define BINDERBUILDER_H

#include <QObject>
#include <QTreeWidget>

#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QMessageBox>
#include <QUuid>

#include "xmlhandler.h"
#include "tools/constants.h"

class BinderTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit BinderTreeWidget(QWidget *parent = nullptr);
    void saveXML();
    void parseXML(QDomDocument binderDocument);

signals:

public slots:
    void addItem();
    void deleteSelectedBinder();

private:
    void enableDragAndDrop();
    QDomElement parseItem(QTreeWidgetItem *item, QDomDocument root);
    QTreeWidgetItem* createItem(QDomElement node);
};

#endif // BINDERBUILDER_H
