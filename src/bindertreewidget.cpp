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

/**
 * TODO Move this in XMLHandler
 * @brief BinderTreeWidget::loadXML
 */
void BinderTreeWidget::saveXML()
{
    QDomDocument binderDocument;
    QDomElement binderWidget = binderDocument.createElement("binders");
    binderDocument.appendChild(binderWidget);

    for (int i=0; i<this->topLevelItemCount(); i++) {
        binderWidget.appendChild(this->parseItem(this->topLevelItem(i), binderDocument));
    }

    QFile doc_xml;
    doc_xml.setFileName(QString("/Users/david/Documents/Perso/Interrogator/build-Interrogator-Desktop_Qt_5_9_0_clang_64bit-Debug/").append(BINDER_SAVE_FILE));
    doc_xml.open(QIODevice::WriteOnly);
    QTextStream sortie;
    sortie.setDevice(&doc_xml);
    binderDocument.save(sortie, 4);
    doc_xml.close();
}

QDomElement BinderTreeWidget::parseItem(QTreeWidgetItem *item, QDomDocument root)
{
    QDomElement domItem = root.createElement("binder");

    QDomElement domItemTitle = root.createElement("title");
    QDomText domItemTitleText = root.createTextNode(item->text(0));
    domItemTitle.appendChild(domItemTitleText);

    QDomElement domItemDescription = root.createElement("description");
    QDomText domItemDescriptionText = root.createTextNode(item->text(1));
    domItemDescription.appendChild(domItemDescriptionText);

    domItem.appendChild(domItemTitle);
    domItem.appendChild(domItemDescription);

    if (item->childCount() > 0) {
        for (int i = 0; i<item->childCount(); i++) {
            domItem.appendChild(this->parseItem(item->child(i), root));
        }
    }

    return domItem;
}

/**
 * TODO move in XMLHandler
 * @brief BinderTreeWidget::readXML
 * @return
 */
QDomDocument BinderTreeWidget::readXML()
{
    QDomDocument binderDocument;
    QFile file(QString("/Users/david/Documents/Perso/Interrogator/build-Interrogator-Desktop_Qt_5_9_0_clang_64bit-Debug/").append(BINDER_SAVE_FILE));
    if (!file.open(QIODevice::ReadOnly))
        return QDomDocument();
    if (!binderDocument.setContent(&file)){
        file.close();
        return QDomDocument();
    }
    file.close();

    QDomElement racine = binderDocument.documentElement();
    QDomNode noeud = racine.firstChild();

    while (!noeud.isNull()) {
        QDomElement binder = noeud.toElement();

    }

    return QDomDocument();
}
