#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QDomDocument>
#include <QFile>
#include <QString>
#include <QTextStream>

class XMLHandler
{
public:
    XMLHandler();

    bool saveXML(QDomDocument source, QString filename);
    QDomDocument readXML(QString filename);
    QString getLastError();
    bool hasError();

private:
    void setLastError(QString error);

    bool error = false;
    QString lastError = QString();
};

#endif // XMLHANDLER_H
