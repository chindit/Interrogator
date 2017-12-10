#ifndef BINDEREDITIONDIALOG_H
#define BINDEREDITIONDIALOG_H

#include <QDialog>
#include <QMap>
#include <QMessageBox>
#include <QModelIndex>

#include "binder/bindermodel.h"
#include "tools/constants.h"

namespace Ui {
class BinderEditionDialog;
}

class BinderEditionDialog : public QDialog
{
    Q_OBJECT

signals:
    void editionFinished(QString title, QString description);

public:
    explicit BinderEditionDialog(QWidget *parent = 0);
    ~BinderEditionDialog();
    void editBinder(QString title, QString text);

public slots:
    void accept();

private:
    void clearData();

    Ui::BinderEditionDialog *ui;
};

#endif // BINDEREDITIONDIALOG_H
