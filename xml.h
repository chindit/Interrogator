#ifndef XML_H
#define XML_H

#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QMultiMap>
#include <QObject>
#include <QTextStream>
#include <QtXml/QDomDocument>

#include "settingsmanager.h"

class Xml : public QObject
{
    Q_OBJECT
public:
    explicit Xml(QObject *parent = 0);
    ~Xml();
    QList<QMultiMap<QString, QString> > getCategories();
    QList<QMultiMap<QString, QString> > getQuestions(int id);
    QList<QMultiMap<QString, QString> > getQuestions(QString nom);
    QMultiMap<QString, QString> getQuestion(int categ, int question);
    QMultiMap<QString, QString> getCategorie(int categ);
    void setData(QMultiMap<QString, QString> data);
    int getLastId(int categ = 0);
    int getCategId(QString nom);
    void deleteData(bool isCateg, int id, int id2 = 0);

signals:

public slots:
    void exportData();
    void openBase();

signals:
    void goRestart();

private:
    void readData();
    void saveData(QString nom = "");

    QList< QMultiMap<QString,QString> > donnees;
    bool updated;
    SettingsManager *manager;

};

#endif // XML_H
