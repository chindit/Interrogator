#ifndef PDF_H
#define PDF_H

#include <QDir>
#include <QTextDocument>
#include <QFont>
#include <QObject>
#include <QPainter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <math.h>

#include "xml.h"

class Pdf : public QObject
{
    Q_OBJECT
public:
    explicit Pdf(QObject *parent = 0);
    void generate(QList<QMultiMap<QString,QString> > questions, QMultiMap<QString,QString> infos, bool prof);

signals:

public slots:

};

#endif // PDF_H
