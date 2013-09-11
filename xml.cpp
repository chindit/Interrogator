#include "xml.h"

Xml::Xml(QObject *parent) : QObject(parent){
    updated = false;
    manager = new SettingsManager;
    if(manager->getSettings(Base).toString().isEmpty()){
        QMessageBox::warning(0, "Aucun fichier de base", "Attention, aucun fichie de base n'a été détecté.  <em>Interrogator</em> ne peut fonctionner normalement sans ce fichier");
        return;
    }
    this->readData();
}

Xml::~Xml(){
    this->saveData();
}

void Xml::saveData(QString nom){
    if((updated || !nom.isEmpty()) && !manager->getSettings(Base).toString().isEmpty()){
        if(manager->getSettings(FullSecurity).toBool())
            this->securitySave();
        QDomDocument doc;
        QDomElement livres = doc.createElement("interrogator");
        doc.appendChild(livres);

        for(int i=0; i<donnees.size(); i++){
            QMultiMap<QString, QString> livreActuel = donnees.at(i);
            QDomElement livre;
            if(livreActuel.value("type") == "categorie"){
                livre = doc.createElement("categorie");
            }
            if(livreActuel.value("type") == "question"){
                livre = doc.createElement("question");
                if(livreActuel.value("qcm") == "true"){
                    QMultiMap<QString, QString>::iterator it;
                    QList < int > reponsesCorrectes;
                    for(it = livreActuel.begin(); it != livreActuel.end(); ++it){
                        if(it.key() == "valide")
                            reponsesCorrectes.append(QString(it.value()).toInt());
                    }
                    //QStringList reponsesCorrectes = livreActuel.values()
                    for(it = livreActuel.begin(); it != livreActuel.end(); ++it){
                        bool convert = false;
                        int id = 0;
                        id = QString(it.key()).toInt(&convert);
                        if(convert){
                            convert = false;
                            QDomElement elem = doc.createElement("reponse");
                            livre.appendChild(elem);
                            elem.setAttribute("id", QString::number(id));
                            if(reponsesCorrectes.contains(id))
                                elem.setAttribute("correct", "true");
                            QDomText reponse = doc.createTextNode(it.value());
                            elem.appendChild(reponse);
                        }
                    }
                }
            }
            livres.appendChild(livre);
            livre.setAttribute("id", livreActuel.value("id"));

            //Iterator
            QMultiMap<QString, QString>::iterator it;
            for(it = livreActuel.begin(); it != livreActuel.end(); ++it){
                if(it.key() != "id" && QString(it.key()).toInt() == 0 && it.key() != "valide"){
                    QDomElement elem = doc.createElement(it.key());
                    livre.appendChild(elem);
                    QDomText textElem = doc.createTextNode(it.value());
                    elem.appendChild(textElem);
                }
            }
        }

        QFile doc_xml;
        if(!nom.isEmpty()){
            doc_xml.setFileName(nom);
        }
        else{
            doc_xml.setFileName(manager->getSettings(Base).toString());
        }
        if (!doc_xml.open(QIODevice::WriteOnly)){
            return;
        }
        QTextStream sortie;
        sortie.setDevice(&doc_xml);
        doc.save(sortie, 4);
        doc_xml.close();
    }
}

void Xml::exportData(){
    QString nomFichier = QFileDialog::getSaveFileName(0, "Exporter les données", QDir::homePath(), "Fichiers XML (*.xml)");
    if(!nomFichier.isEmpty()){
        if(!nomFichier.endsWith(".xml"))
            nomFichier.append(".xml");
        this->saveData(nomFichier);
    }
    return;
}

void Xml::readData(){
    QDomDocument doc;
    QFile file(manager->getSettings(Base).toString());
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)){
        file.close();
        return;
    }
    file.close();

    int i=0;
    QDomNodeList tab;
    QDomElement livre;
    QDomNode n;
    QDomElement racine = doc.documentElement();
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull()){
        livre = noeud.toElement();
        if(livre.tagName() == "categorie" || livre.tagName() == "question"){
            QMultiMap<QString, QString> elem;

            elem.insert("id", livre.attribute("id"));
            tab = livre.childNodes();
            for(i=0;i<tab.length();i++){
                n = tab.item(i);
                if(n.nodeName() == "reponse" && n.isElement()){
                    if(n.toElement().hasAttribute("id")) //Obligatoirement un QCM
                        elem.insert(n.toElement().attribute("id"), n.firstChild().toText().data());
                    if(n.toElement().hasAttribute("correct"))
                        elem.insert("valide", n.toElement().attribute("id"));
                    else
                        elem.insert(n.nodeName(), n.firstChild().toText().data());
                }
                else
                    elem.insert(n.nodeName(), n.firstChild().toText().data());
            }
            donnees.append(elem);
        }
        noeud = noeud.nextSibling();
    }
    return;
}

QList<QMultiMap<QString, QString> > Xml::getCategories(){
    QList< QMultiMap<QString, QString> > categories;
    for(int i=0; i<donnees.size(); i++)
        if(donnees.at(i).value("type") == "categorie")
            categories.append(donnees.at(i));
    return categories;
}

QList<QMultiMap<QString, QString> > Xml::getQuestions(int id){
    QList<QMultiMap<QString, QString> > questions;
    for(int i=0; i<donnees.size(); i++)
        if(donnees.at(i).value("type") == "question" && donnees.at(i).value("categorie").toInt() == id)
            questions.append(donnees.at(i));
    return questions;
}

void Xml::setData(QMultiMap<QString, QString> data){
    donnees.append(data);
    updated = true;
}

int Xml::getLastId(int categ){
    QList< QMultiMap<QString, QString> > liste;
    int id = 0;
    if(categ == 0)
        liste = this->getCategories();
    else
        liste = this->getQuestions(categ);
    for(int i=0; i<liste.size(); i++)
        if(liste.at(i).value("id").toInt() > id)
            id = liste.at(i).value("id").toInt();
    return id+1;
}

int Xml::getCategId(QString nom){
    QList<QMultiMap<QString, QString> > categories = this->getCategories();
    int id = 0;
    for(int i=0; i<categories.size(); i++)
        if(categories.at(i).value("intitule") == nom)
            id = categories.at(i).value("id").toInt();
    return id;
}

QList<QMultiMap<QString, QString> > Xml::getQuestions(QString nom){
    QList<QMultiMap<QString, QString> > categories;
    int id = this->getCategId(nom);
    if(id == 0)
        return categories;
    return this->getQuestions(id);
}

void Xml::deleteData(bool isCateg, int id, int id2){
    if(isCateg){
        for(int i=0; i<donnees.size(); i++){
            if(donnees.at(i).value("type") == "categorie")
                if(donnees.at(i).value("id").toInt() == id){
                    donnees.removeAt(i);
                    updated = true;
                }
            if(donnees.at(i).value("type") == "question")
                if(donnees.at(i).value("categorie").toInt() == id){
                    donnees.removeAt(i);
                    updated = true;
                }
        }
    }
    else{
        for(int i=0; i<donnees.size(); i++){
            if(donnees.at(i).value("type") == "question" && donnees.at(i).value("categorie").toInt() == id && donnees.at(i).value("id").toInt() == id2){
                donnees.removeAt(i);
                updated = true;
            }
        }
    }
}

QMultiMap<QString,QString> Xml::getQuestion(int categ, int question){
    QList<QMultiMap<QString, QString> > questions = this->getQuestions(categ);
    for(int i=0; i<questions.size(); i++)
        if(questions.at(i).value("id").toInt() == question)
            return questions.at(i);
    QMultiMap<QString, QString> vacio;
    return vacio;
}

QMultiMap<QString, QString> Xml::getCategorie(int categ){
    QList<QMultiMap<QString,QString> > categories = this->getCategories();
    for(int i=0; i<categories.size(); i++)
        if(categories.at(i).value("id").toInt() == categ)
            return categories.at(i);
    QMultiMap<QString, QString> vacio;
    return vacio;
}

void Xml::openBase(){
    QString nomFichier = QFileDialog::getOpenFileName(0, "Répertoire de la base", QDir::homePath(), "Fichiers XML (*.xml)");
    if(!nomFichier.endsWith(".xml")){
        return;
    }
    SettingsManager manager;
    manager.setSettings(Base, nomFichier);
    QMessageBox alerte;
    alerte.setText("La base de question a bien été chargée mais elle ne sera disponible qu'après un redémarrage d'Interrogator");
    alerte.setWindowTitle("Base chargée");
    alerte.setIcon(QMessageBox::Information);
    alerte.setWindowIcon(QIcon(":/icones/images/interrogator.png"));
    alerte.addButton("Redémarrer", QMessageBox::AcceptRole);
    alerte.addButton("On verra plus tard...", QMessageBox::RejectRole);
    int reponse = alerte.exec();
    if(reponse == QMessageBox::AcceptRole){
        emit goRestart();
    }
    return;
}

void Xml::securitySave(){
    QString dossier = QDir::homePath();
#ifdef Q_OS_WIN
    dossier += "/AppData/Local/Interrogator";
#elif defined(Q_WS_MAC)
    dossier += "/Library/Application Support/Interrogator";
#else
    dossier += "/.Interrogator";
#endif
    QDir dirSave = QDir(dossier);
    QStringList fichiers = dirSave.entryList();
    fichiers.removeFirst();
    fichiers.removeFirst();
    QDateTime currentDate = QDateTime::currentDateTime();
    if(fichiers.size() >= 10){
        QFile fichier(dossier+"/"+fichiers.at(0));
        if(!fichier.remove())
            qDebug() << fichier.errorString();
    }
    QString nom = "autosave_"+currentDate.toString("ddMM_hhmmss")+".xml";
    QFile actualBase(manager->getSettings(Base).toString());
    actualBase.copy(dossier+"/"+nom);
    return;
}
