#include "bindertreewidget.h"

BinderTreeWidget::BinderTreeWidget(QWidget *parent) : QTreeWidget()
{
    this->setParent(parent);

    this->enableDragAndDrop();

    XMLHandler handler = XMLHandler();
    this->parseXML(handler.readXML("/tmp/binder.xml"));

    /*QTreeWidgetItem *tIt = new QTreeWidgetItem();
    tIt->setText(0, "Hey! Parent");
    QTreeWidgetItem *tSec = new QTreeWidgetItem();
    tSec->setText(0, "Buh, second");
    tIt->addChild(tSec);
    QTreeWidgetItem *tIt2 = new QTreeWidgetItem();
    tIt2->setText(0, "New Root");
    tIt2->setText(1, "Hey description!");
    this->addTopLevelItem(tIt);
    this->addTopLevelItem(tIt2);*/
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

void BinderTreeWidget::addItem()
{
    QTreeWidgetItem *newLine = new QTreeWidgetItem();
    newLine->setText(0, "Nouveau classeur");
    newLine->setText(2, QUuid::createUuid().toString());

    if (this->selectedItems().count() > 0) {
        this->selectedItems().at(0)->addChild(newLine);
    } else {
        this->addTopLevelItem(newLine);
    }
    this->resizeColumnToContents(0);
}

/**
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

    XMLHandler handler = XMLHandler();
    handler.saveXML(binderDocument, "/tmp/binder.xml");

    /*QFile doc_xml;
    doc_xml.setFileName(QString("/tmp/").append(BINDER_SAVE_FILE));
    doc_xml.open(QIODevice::WriteOnly);
    QTextStream sortie;
    sortie.setDevice(&doc_xml);
    binderDocument.save(sortie, 4);
    doc_xml.close();*/
}

QDomElement BinderTreeWidget::parseItem(QTreeWidgetItem *item, QDomDocument root)
{
    QDomElement domItem = root.createElement("binder");
    domItem.setAttribute("id", item->text(2));

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
 * @brief BinderTreeWidget::readXML
 * @return
 */
void BinderTreeWidget::parseXML(QDomDocument binderDocument)
{
    /*QDomDocument binderDocument;
    QFile file(QString("/tmp/").append(BINDER_SAVE_FILE));
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!binderDocument.setContent(&file)){
        file.close();
        return;
    }
    file.close();*/

    QDomElement racine = binderDocument.documentElement();

    racine.childNodes();
    QDomNodeList children = racine.childNodes();
    int items = children.count();
    for (int i = 0; i < items; i++) {
        QDomNode noeud = children.at(i);
        this->addTopLevelItem(this->createItem(noeud.toElement()));
        noeud.nextSibling();
    }

    return;
}

QTreeWidgetItem* BinderTreeWidget::createItem(QDomElement node)
{
    QTreeWidgetItem *currentItem = new QTreeWidgetItem();
    currentItem->setText(2, node.attribute("id"));
    QDomNodeList children = node.childNodes();
    int items = children.count();
    for (int i = 0; i < items; i++) {
        QDomNode subNode = children.at(i);
        if (subNode.nodeName().compare("title") == 0) {
            currentItem->setText(0, subNode.toElement().text());
        } else if (subNode.nodeName().compare("description") == 0) {
            currentItem->setText(1, subNode.toElement().text());
        } else if (subNode.nodeName().compare("binder") == 0) {
            currentItem->addChild(this->createItem(subNode.toElement()));
        } else {
            // ERROR
            return currentItem;
        }
    }

    return currentItem;
}
