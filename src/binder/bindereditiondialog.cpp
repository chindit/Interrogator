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
    result.insert("title", ui->lineEditTitle->text());
    result.insert("description", ui->plainTextEditDescription->toPlainText());
    result.insert("parent", ui->comboBoxParent->currentText().right(ui->comboBoxParent->currentText().indexOf('>')));

    emit editionFinished(result);

    this->clearData();

    this->hide();
}

void BinderEditionDialog::clearData()
{
    ui->lineEditTitle->clear();
    ui->plainTextEditDescription->clear();
}

void BinderEditionDialog::setTitleHierarchy(QStringList hierarchy)
{
    hierarchy.prepend(EMPTY_MARKER);
    ui->comboBoxParent->addItems(hierarchy);
}

void BinderEditionDialog::editBinder(QVariant title, QVariant text, QVariant parent)
{
    ui->lineEditTitle->setText(title.toString());
    ui->plainTextEditDescription->clear();
    ui->plainTextEditDescription->appendPlainText(text.toString());

    int requestedIndex = ui->comboBoxParent->findText(parent.toString(), Qt::MatchContains);
    ui->comboBoxParent->setCurrentIndex((requestedIndex > 0) ? requestedIndex : 0);

    this->show();
}
