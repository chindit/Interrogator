#include "bindertreewidget.h"

BinderTreeWidget::BinderTreeWidget(QWidget *parent) : QTreeWidget()
{
    this->setParent(parent);

    this->enableDragAndDrop();

    QTreeWidgetItem *tIt = new QTreeWidgetItem();
    tIt->setText(0, "Hey! Parent");
    QTreeWidgetItem *tSec = new QTreeWidgetItem();
    tSec->setText(0, "Buh, second");
    tIt->addChild(tSec);
    QTreeWidgetItem *tIt2 = new QTreeWidgetItem();
    tIt2->setText(0, "New Root");
    tIt2->setText(1, "Hey description!");
    this->addTopLevelItem(tIt);
    this->addTopLevelItem(tIt2);
}

void BinderTreeWidget::enableDragAndDrop()
{
    this->setDragEnabled(true);
    this->viewport()->setAcceptDrops(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDropIndicatorShown(true);
    this->setAcceptDrops(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);
}
