#ifndef BINDEREDITIONDIALOG_H
#define BINDEREDITIONDIALOG_H

#include <QDialog>
#include <QMap>
#include <QMessageBox>

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

public slots:
    void accept();

private:
    void clearData();

    Ui::BinderEditionDialog *ui;
    int binderId = 0;
};

#endif // BINDEREDITIONDIALOG_H
