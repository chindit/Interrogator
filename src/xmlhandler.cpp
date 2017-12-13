#include "xmlhandler.h"

XMLHandler::XMLHandler()
{

}

bool XMLHandler::saveXML(QDomDocument source, QString filename)
{
    if (source.isNull()) {
        this->setLastError("DomDocument is null");
        return false;
    }

    QFile *target = new QFile(filename);
    if (!target->open(QIODevice::WriteOnly)) {
        this->setLastError(QString("Unable to open save file.  Error returned is following %s").arg(target->errorString()));
        return false;
    }

    QTextStream output;
    output.setDevice(target);
    source.save(output, 4);

    target->close();
    delete target;

    return true;
}

QDomDocument XMLHandler::readXML(QString filename)
{
    QFile *target = new QFile(filename);

    if (!target->exists()) {
        this->setLastError("Requested file is not readable");
        return QDomDocument();
    }

    if (!target->open(QIODevice::ReadOnly)) {
        this->setLastError(QString("Unable to open file «%s».  Returned error is following: %s")
                           .arg(filename).arg(target->errorString()));
        return QDomDocument();
    }

    QDomDocument content = QDomDocument();
    if (!content.setContent(target)){
        this->setLastError(QString("File «%s» does not contain valid XML!").arg(filename));
        target->close();
        delete target;

        return content;
    }
    target->close();
    delete target;

    return content;
}

bool XMLHandler::hasError()
{
    return this->error;
}

QString XMLHandler::getLastError()
{
    return this->lastError;
}

void XMLHandler::setLastError(QString error)
{
    this->lastError = error;
    this->error = true;
}
