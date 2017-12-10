#include "bindereditiondialog.h"
#include "ui_bindereditiondialog.h"

BinderEditionDialog::BinderEditionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BinderEditionDialog)
{
    ui->setupUi(this);
}

BinderEditionDialog::~BinderEditionDialog()
{
    delete ui;
}

void BinderEditionDialog::accept()
{
    if (ui->lineEditTitle->text().length() < 5) {
        QMessageBox::warning(this, tr("Titre invalide"), tr("Le titre doit contenir au moins 5 caractères"));
        return;
    }

    emit editionFinished(ui->lineEditTitle->text(), ui->plainTextEditDescription->toPlainText());

    this->clearData();

    this->hide();
}

void BinderEditionDialog::clearData()
{
    ui->lineEditTitle->clear();
    ui->plainTextEditDescription->clear();
}

void BinderEditionDialog::editBinder(QString title, QString text)
{
    ui->lineEditTitle->setText(title);
    ui->plainTextEditDescription->clear();
    ui->plainTextEditDescription->appendPlainText(text);

    this->show();
}
