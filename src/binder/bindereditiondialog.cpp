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
    QMap<QString, QString> result = QMap<QString, QString>();
    result.insert("id", QString::number(this->binderId));
    result.insert("title", ui->lineEditTitle->text());
    result.insert("description", ui->plainTextEditDescription->toPlainText());

    emit editionFinished(result);

    this->clearData();
}

void BinderEditionDialog::clearData()
{
    ui->lineEditTitle->clear();
    ui->plainTextEditDescription->clear();
    this->binderId = 0;
}

void BinderEditionDialog::setTitleHierarchy(QStringList hierarchy)
{
    ui->comboBoxParent->addItems(hierarchy);
}
