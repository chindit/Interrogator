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
    void editionFinished(QMap<QString, QString> binder);

public:
    explicit BinderEditionDialog(QWidget *parent = 0);
    ~BinderEditionDialog();
    void setTitleHierarchy(QStringList hierarchy);
    void editBinder(QVariant title, QVariant text, QVariant parent);

public slots:
    void accept();

private:
    void clearData();

    Ui::BinderEditionDialog *ui;
};

#endif // BINDEREDITIONDIALOG_H
