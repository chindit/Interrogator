#include "toolsdialog.h"
#include "ui_toolsdialog.h"

toolsDialog::toolsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::toolsDialog)
{
    ui->setupUi(this);
    ui->checkBox_next->setVisible(false);
}

toolsDialog::~toolsDialog()
{
    delete ui;
}

void toolsDialog::openBase(){
    QString nomFichier = QFileDialog::getExistingDirectory(0, "Répertoire de la base", QDir::homePath());
    if(nomFichier.isEmpty()){
        QMessageBox::critical(this, "Pas de fichier choisi", "<strong>ATTENTION:</strong> vous n'avez pas choisi de fichier de données.  <em>Interrogator</em> ne pourra pas fonctionner normalement!");
        return;
    }
    if(!nomFichier.endsWith("/")){
        nomFichier.append("/");
    }
    nomFichier.append("interrogator.xml");
    SettingsManager manager;
    manager.setSettings(Base, nomFichier);
    emit baseUpdated();
    this->close();
    return;
}
