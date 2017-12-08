#include "interrogator.h"
#include "ui_interrogator.h"

Interrogator::Interrogator(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interrogator){
    this->buildUI();

    insManager = new SettingsManager;
    insDialog = new ToolsDialog(this);
    connect(insDialog, SIGNAL(baseUpdated()), insManager, SLOT(update()));

    //Vérification des settings
    if(insManager->getSettings(Base).toString().isEmpty()){
        insDialog->exec();
    }

    insXml = new Xml;
    edit = false;
    idEdit = 0;
    idEditCateg = 0;
    QSignalMapper *mappeur[5];
    for(int i=0; i<5; i++)
        mappeur[i] = new QSignalMapper;

    //----------------------------------------------
    //0 - Accueil
    //----------------------------------------------
    /*connect(ui->pushButton_category, SIGNAL(clicked()), mappeur[0], SLOT(map()));
    mappeur[0]->setMapping(ui->pushButton_category, 1);
    connect(ui->pushButton_add_category, SIGNAL(clicked()), mappeur[1], SLOT(map()));
    mappeur[1]->setMapping(ui->pushButton_add_category, 2);
    connect(ui->pushButton_question, SIGNAL(clicked()), mappeur[2], SLOT(map()));
    mappeur[2]->setMapping(ui->pushButton_question, 3);
    connect(ui->pushButton_add_question_2, SIGNAL(clicked()), mappeur[3], SLOT(map()));
    mappeur[3]->setMapping(ui->pushButton_add_question_2, 4);
    connect(ui->pushButton_interro, SIGNAL(clicked()), mappeur[4], SLOT(map()));
    mappeur[4]->setMapping(ui->pushButton_interro, 5);
    for(int i=0;i<5;i++)
        connect(mappeur[i], SIGNAL(mapped(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));*/
    //----------------------------------------------
    //0 - Menu
    //----------------------------------------------
    ui->actionEmptyAnswers->setChecked(insManager->getSettings(EmptyAnswers).toBool());
    ui->actionFullSecurity->setChecked(insManager->getSettings(FullSecurity).toBool());
    ui->actionEmptyQuestions->setChecked(insManager->getSettings(EmptyQCMQuestion).toBool());
    connect(ui->actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->action_propos_de_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_propos_d_Interrogator, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionSauvegarder_les_donn_es, SIGNAL(triggered()), insXml, SLOT(exportData()));
    connect(ui->actionOuvrir_une_base, SIGNAL(triggered()), insXml, SLOT(openBase()));
    connect(insXml, SIGNAL(goRestart()), this, SLOT(restart()));
    connect(ui->actionEmptyAnswers, SIGNAL(triggered()), this, SLOT(changeStatusAnswer()));
    connect(ui->actionEmptyQuestions, SIGNAL(triggered()), this, SLOT(changeStatusQuestion()));

    QStringList dossiers = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    QString dossier = dossiers.first();
    QDir dir_dossier(dossier);
    if(!dir_dossier.exists()){
        dir_dossier.mkdir(dossier);
    }
    connect(ui->actionFullSecurity, SIGNAL(triggered()), this, SLOT(setFullSecurity()));

    //----------------------------------------------
    //1 - Liste des catégories
    //----------------------------------------------
    QList<QMultiMap<QString, QString> > categories = insXml->getCategories();
    this->setListCateg();

    //----------------------------------------------
    //2 - Ajouter une catégorie
    //----------------------------------------------
    connect(ui->pushButton_cancel_2, SIGNAL(clicked()), this, SLOT(undoCateg()));

    //----------------------------------------------
    //3 - Gérer les questions
    //----------------------------------------------
    for(int i=0; i<categories.size(); i++)
        ui->comboBox_categorie->addItem(categories.at(i).value("intitule"));
    this->updateList();
    QSignalMapper *mappeur4 = new QSignalMapper;
    connect(ui->pushButton_add_question_3, SIGNAL(clicked()), mappeur4, SLOT(map()));
    mappeur4->setMapping(ui->pushButton_add_question_3, "4");
    connect(mappeur4, SIGNAL(mapped(QString)), this, SLOT(changeStacked(QString)));
    //----------------------------------------------
    //4 - Ajouter une question
    //----------------------------------------------
    ui->label_actual_category->setText(ui->comboBox_categorie->currentText());
    for(int i=0; i<categories.size(); i++)
        ui->comboBox_categories->addItem(categories.at(i).value("intitule"));
    this->setQCMStatus();
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(undoQuestion()));
    //----------------------------------------------
    //5 - Générer un PDF
    //----------------------------------------------
    for(int i=0; i<categories.size(); i++)
        ui->comboBox_categories_2->addItem(categories.at(i).value("intitule"));
    ui->dateEdit->setVisible(false);
    ui->lineEdit_titre_interro->setVisible(false);
    ui->radioButton_print->setVisible(false);
    ui->radioButton_pdf->setChecked(true);
}

Interrogator::~Interrogator()
{
    delete binderEditionDialog;
    delete ui;
    delete insXml;
    delete insDialog;
}

/**
 * @brief Interrogator::buildUI
 */
void Interrogator::buildUI(){
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    QSignalMapper *mapper[2];
    for(int i=0; i<2; i++)
        mapper[i] = new QSignalMapper;

    //----------------------------------------------
    //0 - Home
    //----------------------------------------------
    connect(ui->pushButtonBinder, SIGNAL(clicked()), mapper[0], SLOT(map()));
    mapper[0]->setMapping(ui->pushButtonBinder, 1);

    //----------------------------------------------
    //1 - Binder
    //----------------------------------------------
    connect(ui->pushButtonBinderHome, SIGNAL(clicked()), mapper[1], SLOT(map()));
    mapper[1]->setMapping(ui->pushButtonBinderHome, 0);

    this->binderEditionDialog = new BinderEditionDialog(this);
    connect(ui->pushButtonBinderAdd, SIGNAL(clicked()), this->binderEditionDialog, SLOT(show()));

    // Connect mappers to QStackedWidget
    for(int i=0;i<2;i++)
        connect(mapper[i], SIGNAL(mapped(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

    // Labels will not be shown if they are set in widget
    QStringList labels; labels << tr("Titre") << tr("Description");
    ui->treeWidgetBinder->setHeaderLabels(labels);
    ui->treeWidgetBinder->expandAll();
    ui->treeWidgetBinder->resizeColumnToContents(0);
    ui->treeWidgetBinder->show();

    ui->treeWidgetBinder->saveXML();
}

/**
 * @brief Interrogator::binderSelectionUpdated
 * @param selectedIndex
 */
void Interrogator::binderSelectionUpdated(QModelIndex selectedIndex)
{
    this->currentSelectedIndex = selectedIndex;
    ui->pushButtonBinderDelete->setEnabled(true);
    ui->pushButtonBinderEdit->setEnabled(true);
}

/**
 * @brief Interrogator::editBinder
 */
void Interrogator::editBinder()
{
    if (!ui->pushButtonBinderEdit->isEnabled() || !this->currentSelectedIndex.isValid()) {
        return;
    }
    BinderItem *item = this->binderTree->getItem(this->currentSelectedIndex);
    QVariant parent = (item->getParentItem() == NULL) ? QVariant() : item->getParentItem()->data(0);
    // TODO fix this with thanslations
    if (parent.toString().compare("Titre") == 0) {
        parent = QVariant('/');
    }
    this->binderEditionDialog->editBinder(item->data(0), item->data(1), parent);
}

/**
 * @brief Interrogator::processBinderEdition
 * @param binder
 */
void Interrogator::processBinderEdition(QMap<QString, QString> binder)
{
    if (!this->currentSelectedIndex.isValid()) {
        // TODO
        return;
    }
    // Edit current binder
    BinderItem *item = this->binderTree->getItem(this->currentSelectedIndex);
    item->setData(0, binder.value("title"));
    item->setData(1, binder.value("description"));

    // TODO handle parent
    this->binderTree->updateItemParent(item, (item->getParentItem() == NULL) ? QString() : item->getParentItem()->data(0).toString(), binder.value("parent"));

    // TODO handle update
}



void Interrogator::setListCateg(){
    //Met la liste des catégories à jour
    QStringList headers; headers << "Id" << "Titre" << "Description" << "Éditer" << "Supprimer";
    ui->tableWidget_categories->setHorizontalHeaderLabels(headers);
    ui->tableWidget_categories->setColumnCount(5);
    ui->tableWidget_categories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QList<QMultiMap<QString, QString> > categories = insXml->getCategories();
    for(int i=0; i<categories.size(); i++){
        ui->tableWidget_categories->insertRow(i);
        QTableWidgetItem *item[5];
        for(int j=0; j<5; j++)
            item[j] = new QTableWidgetItem();
        item[0]->setText(categories.at(i).value("id"));
        item[1]->setText(categories.at(i).value("intitule"));
        item[2]->setText(categories.at(i).value("description"));
        QPixmap editer, supprimer;
        editer.load(":/icones/images/edit.png"); supprimer.load(":/icones/images/delete.png");
        editer = editer.scaled(16, 16); supprimer = supprimer.scaled(16,16);
        item[3]->setData(Qt::DecorationRole, editer);
        item[4]->setData(Qt::DecorationRole, supprimer);
        for(int j=0; j<5; j++)
            ui->tableWidget_categories->setItem(i, j, item[j]);
    }
    ui->tableWidget_categories->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QSignalMapper *mappeur2 = new QSignalMapper;
    connect(ui->pushButton_add_category_2, SIGNAL(clicked()), mappeur2, SLOT(map()));
    mappeur2->setMapping(ui->pushButton_add_category_2, "2");
    connect(mappeur2, SIGNAL(mapped(QString)), this, SLOT(changeStacked(QString)));
}

void Interrogator::updateListCateg(){
    QList<QMultiMap<QString, QString> > categories = insXml->getCategories();
    ui->comboBox_categorie->clear();
    ui->comboBox_categories->clear();
    ui->comboBox_categories_2->clear();
    ui->tableWidget_categories->clear();
    ui->tableWidget_categories->setRowCount(0);
    this->setListCateg();
    for(int i=0; i<categories.size(); i++){
        ui->comboBox_categorie->addItem(categories.at(i).value("intitule"));
        ui->comboBox_categories->addItem(categories.at(i).value("intitule"));
        ui->comboBox_categories_2->addItem(categories.at(i).value("intitule"));
    }
}

void Interrogator::changeStacked(QString widget){
    int nombre = widget.toInt();
    if(nombre < 0 || nombre > 4)
        return;
    ui->stackedWidget->setCurrentIndex(nombre);
    return;
}

void Interrogator::saveCateg(){
    if(ui->lineEdit_nom->text().isEmpty() || ui->plainTextEdit_description->toPlainText().isEmpty()){
        QMessageBox::warning(this, "Champ vide", "Pour continuer, tous les champs doivent être remplis.");
        return;
    }
    int idNew = (idEdit > 0) ? idEdit : insXml->getLastId();
    if(edit && idEdit > 0){
        edit = false;
        insXml->deleteData(true, idEdit);
        idEdit = 0;
        ui->pushButton->setIcon(QIcon(":/icons/images/add.png"));
        ui->pushButton->setText("Ajouter");
    }
    QMultiMap<QString, QString> categorie;
    categorie.insert("type", "categorie");
    categorie.insert("id", QString::number(idNew));
    categorie.insert("intitule", ui->lineEdit_nom->text().trimmed());
    categorie.insert("description", ui->plainTextEdit_description->toPlainText().trimmed());
    insXml->setData(categorie);

    //On nettoye les champs
    ui->lineEdit_nom->clear();
    ui->plainTextEdit_description->clear();

    QMessageBox::information(this, "Catégorie sauvegardée", "La catégorie a été correctement créée");
    ui->stackedWidget->setCurrentIndex(0);
    this->updateListCateg();
    return;
}

void Interrogator::updateList(){
    QList< QMultiMap<QString, QString> > questions = insXml->getQuestions(ui->comboBox_categorie->currentText());
    ui->tableWidget_questions->clear();
    QStringList headers; headers << "Id" << "Id cat" << "Intitule" << "QCM" << "Éditer" << "Supprimer";
    ui->tableWidget_questions->setHorizontalHeaderLabels(headers);
    ui->tableWidget_questions->setColumnCount(6);
    ui->tableWidget_questions->setRowCount(questions.size());
    ui->tableWidget_questions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0; i<questions.size(); i++){
        QTableWidgetItem *item[6];
        for(int j=0; j<6; j++)
            item[j] = new QTableWidgetItem();
        item[0]->setText(questions.at(i).value("id"));
        item[1]->setText(questions.at(i).value("categorie"));
        item[2]->setText(questions.at(i).value("intitule"));
        item[3]->setText(questions.at(i).value("qcm"));
        QPixmap editer, supprimer;
        editer.load(":/icones/images/edit.png"); supprimer.load(":/icones/images/delete.png");
        editer = editer.scaled(16, 16); supprimer = supprimer.scaled(16,16);
        item[4]->setData(Qt::DecorationRole, editer);
        item[5]->setData(Qt::DecorationRole, supprimer);
        for(int j=0; j<6; j++)
            ui->tableWidget_questions->setItem(i, j, item[j]);
    }
    ui->tableWidget_questions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //On met à jour l'onglet d'ajout de questions
    ui->label_actual_category->setText(ui->comboBox_categorie->currentText());
    return;
}

void Interrogator::categoryActivated(int row, int col){
    if(col < 3)
        return;
    int categorie = ui->tableWidget_categories->item(row, 0)->text().toInt();
    if(col == 3){ //Édition
        this->setCategory(categorie);
        ui->stackedWidget->setCurrentIndex(2);
    }
    if(col == 4){ //Supression
        int reponse = QMessageBox::question(this, "Supprimer la catégorie", "Êtes-vous certain(e) de vouloir supprimer cette catégorie?\nToutes les questions en faisant partie seront également supprimées", QMessageBox::Yes | QMessageBox::No);
        if(reponse == QMessageBox::Yes){
            insXml->deleteData(true, categorie);
            this->updateListCateg();
        }
        else{
            return;
        }
    }
    return;
}

void Interrogator::setCategory(int category){
    QList<QMultiMap<QString, QString> > categories = insXml->getCategories();
    for(int i=0; i<categories.size(); i++){
        if(categories.at(i).value("id").toInt() == category){
            ui->lineEdit_nom->setText(categories.at(i).value("intitule"));
            ui->plainTextEdit_description->insertPlainText(categories.at(i).value("description"));
            ui->pushButton->setIcon(QIcon(":/icones/images/edit.png"));
            ui->pushButton->setText("Éditer");
            edit = true;
            idEdit = category;
            return;
        }
    }
    return;
}

void Interrogator::categoryActivatedQuestion(int row, int col){
    if(col < 4)
        return;
    int question = ui->tableWidget_questions->item(row, 0)->text().toInt();
    int categorie = ui->tableWidget_questions->item(row, 1)->text().toInt();
    if(col == 4){ //Édition
        this->setQuestion(question, categorie);
        ui->stackedWidget->setCurrentIndex(4);
    }
    if(col == 5){ //Supression
        int reponse = QMessageBox::question(this, "Supprimer la question", "Êtes vous certain(e) de vouloir supprimer cette question?");
        if(reponse == QMessageBox::Yes){
            insXml->deleteData(false, categorie, question);
            this->updateList();
        }
    }
    return;
}

void Interrogator::setQuestion(int question, int categorie){
    QMultiMap<QString, QString> mapQuestion = insXml->getQuestion(categorie, question);
    ui->label_actual_category->setText(insXml->getCategorie(categorie).value("intitule"));
    ui->plainTextEdit_intitule->insertPlainText(mapQuestion.value("intitule"));
    if(mapQuestion.value("qcm") == "true"){
        ui->checkBox_qcm->setChecked(true);
        ui->stackedWidget_qcm->setCurrentIndex(1);
        ui->spinBox_ponctuation_qcm->setValue(mapQuestion.value("ponctuation", "0").toInt());
        QMultiMap<QString, QString>::iterator it;
        QList < int > reponsesCorrectes;
        for(it = mapQuestion.begin(); it != mapQuestion.end(); ++it){
            if(it.key() == "valide")
                reponsesCorrectes.append(QString(it.value()).toInt());
        }
        for(it = mapQuestion.begin(); it != mapQuestion.end(); ++it){
            bool convert = false;
            int id = 0;
            id = QString(it.key()).toInt(&convert);
            if(convert){
                convert = false;
                QListWidgetItem *item = new QListWidgetItem(it.value());
                item->setFlags(item->flags() |= Qt::ItemIsEditable);
                item->setCheckState(Qt::Unchecked);
                if(reponsesCorrectes.contains(id))
                    item->setCheckState(Qt::Checked);
                ui->listWidget_qcm->addItem(item);
            }
        }
    }
    else{
        ui->textEdit_reponse->insertPlainText(mapQuestion.value("reponse"));
        ui->spinBox_lignes_reponse->setValue(mapQuestion.value("lignes", "0").toInt());
        ui->spinBox_ponctuation->setValue(mapQuestion.value("ponctuation", "0").toInt());
    }
    edit = true;
    idEdit = question;
    idEditCateg = categorie;
    ui->pushButton_add_question->setText("Éditer");
    ui->pushButton_add_question->setIcon(QIcon(":/icones/images/edit.png"));
    return;
}

void Interrogator::setQCMStatus(){
    if(ui->checkBox_qcm->isChecked())
        ui->stackedWidget_qcm->setCurrentIndex(1);
    else
        ui->stackedWidget_qcm->setCurrentIndex(0);
    return;
}

void Interrogator::addQCMAnswer(){
    QListWidgetItem *item = new QListWidgetItem("Réponse");
    item->setFlags(item->flags() |= Qt::ItemIsEditable);
    item->setCheckState(Qt::Unchecked);
    ui->listWidget_qcm->addItem(item);
    return;
}

void Interrogator::saveQuestion(){
    bool vide = false;
    if((ui->plainTextEdit_intitule->toPlainText().isEmpty() && !ui->checkBox_qcm->isChecked()) || (ui->plainTextEdit_intitule->toPlainText().isEmpty() && ui->checkBox_qcm->isChecked() && !insManager->getSettings(EmptyQCMQuestion).toBool()) || ui->label_actual_category->text().isEmpty())
        vide = true;
    if(ui->checkBox_qcm->isChecked()){
        if(ui->listWidget_qcm->count() == 0)
            vide = true;
    }
    else
        if(ui->textEdit_reponse->toPlainText().isEmpty() && !insManager->getSettings(EmptyAnswers).toBool())
            vide = true;
    if(vide){
        QMessageBox::warning(this, "Champ vide", "Pour continuer, tous les champs doivent être remplis.");
        return;
    }
    int idNew = (idEdit > 0) ? idEdit : insXml->getLastId();
    if(edit && idEdit > 0){
        edit = false;
        if(idEditCateg != insXml->getCategId(ui->label_actual_category->text())){
            //Il y a eu un changement de categorie
            idNew = insXml->getLastId(insXml->getCategId(ui->label_actual_category->text()));
        }
        insXml->deleteData(false, idEditCateg, idEdit);
        idEdit = 0;
        idEditCateg = 0;
        ui->pushButton_add_question->setIcon(QIcon(":/icons/images/add.png"));
        ui->pushButton_add_question->setText("Ajouter");
    }
    else
        idNew = insXml->getLastId(insXml->getCategId(ui->label_actual_category->text()));
    QMultiMap<QString, QString> question;
    question.insert("type", "question");
    question.insert("id", QString::number(idNew));
    question.insert("intitule", ui->plainTextEdit_intitule->toPlainText().trimmed());
    question.insert("categorie", QString::number(insXml->getCategId(ui->label_actual_category->text().trimmed())));
    if(!ui->checkBox_qcm->isChecked()){
        question.insert("reponse", ui->textEdit_reponse->toPlainText().trimmed());
        question.insert("qcm", "false");
        question.insert("ponctuation", QString::number(ui->spinBox_ponctuation->value()));
        question.insert("lignes", QString::number(ui->spinBox_lignes_reponse->value()));
    }
    else{
        question.insert("qcm", "true");
        question.insert("ponctuation", QString::number(ui->spinBox_ponctuation_qcm->value()));
        for(int i=0; i<ui->listWidget_qcm->count(); i++){
            if(!ui->listWidget_qcm->item(i)->text().isEmpty()){
                question.insert(QString::number(i+1), ui->listWidget_qcm->item(i)->text().trimmed());
                if(ui->listWidget_qcm->item(i)->checkState() == Qt::Checked)
                    question.insert("valide", QString::number(i+1));
            }
        }
    }
    insXml->setData(question);
    QMessageBox::information(this, "Question sauvegardée", "La question a été correctement enregistrée");
    //Clear data
    ui->plainTextEdit_intitule->clear();
    if(ui->checkBox_qcm->isChecked())
        ui->listWidget_qcm->clear();
    else
        ui->textEdit_reponse->clear();
    ui->spinBox_ponctuation->setValue(0);
    ui->spinBox_ponctuation_qcm->setValue(0);
    ui->spinBox_lignes_reponse->setValue(0);
    this->updateList();
    return;
}

void Interrogator::undoQuestion(){
    if(idEdit > 0){
        idEdit = 0;
        idEditCateg = 0;
        edit = false;
        ui->label_actual_category->setText("");
        ui->plainTextEdit_intitule->clear();
        if(ui->checkBox_qcm->isChecked()){
            ui->checkBox_qcm->setChecked(false);
            ui->listWidget_qcm->clear();
            ui->spinBox_ponctuation_qcm->setValue(1);
        }
        else{
            ui->plainTextEdit_description->clear();
            ui->spinBox_lignes_reponse->setValue(0);
            ui->spinBox_ponctuation->setValue(0);
        }
        ui->pushButton_add_question->setIcon(QIcon(":/icons/images/add.png"));
        ui->pushButton_add_question->setText("Ajouter");
    }
    ui->stackedWidget->setCurrentIndex(0);
    return;
}

void Interrogator::undoCateg(){
    if(edit){
        ui->lineEdit_nom->clear();
        ui->plainTextEdit_description->clear();
        edit = false;
        idEdit = 0;
        ui->pushButton->setIcon(QIcon(":/icons/images/add.png"));
        ui->pushButton->setText("Ajouter");
    }
    ui->stackedWidget->setCurrentIndex(0);
    return;
}

void Interrogator::preparePDF(){
    if(ui->comboBox_categories_2->currentText().isEmpty()){
        QMessageBox::warning(this, "Impossible de générer le PDF", "Il est impossible de générer un PDF sans sélectionner une catégorie de questions.");
        return;
    }
    int categorie = insXml->getCategId(ui->comboBox_categories_2->currentText());
    int nbQuestions = ui->spinBox_nb_questions->value();
    bool prof = ui->checkBox_prof->isChecked();
    QList<QMultiMap<QString,QString> > questions = insXml->getQuestions(categorie);
    if(ui->radioButton_pdf_qcm->isChecked() || ui->radioButton_pdf_texte->isChecked()){
        QList<QMultiMap<QString,QString> > qqcm;
        QList<QMultiMap<QString,QString> > qtxt;
        for(int i=0; i<questions.size(); i++){
            if(questions.at(i).value("qcm") == "true"){
                qqcm.append(questions.at(i));
            }
            else
                qtxt.append(questions.at(i));
        }
        if(ui->radioButton_pdf_qcm->isChecked())
            questions = qqcm;
        else
            questions = qtxt;
    }
    if(nbQuestions < questions.size()){
        QList<QMultiMap<QString,QString> > oldQuestions = questions;
        questions.clear();
        for(int i=0; i<nbQuestions; i++){
            int selected = rand()%oldQuestions.size();
            questions.append(oldQuestions.at(selected));
            oldQuestions.removeAt(selected);
        }
    }
    QMultiMap<QString, QString> infos;
    QMultiMap<QString,QString> categ =insXml->getCategorie(categorie);
    if(ui->radioButton_title->isChecked())
        infos.insert("titre", categ.value("intitule"));
    else if(ui->radioButton_description->isChecked())
        infos.insert("titre", categ.value("description"));
    else if(ui->radioButton_titre_description->isChecked()){
        infos.insert("titre", categ.value("intitule"));
        infos.insert("sstitre", categ.value("description"));
    }
    else if(ui->radioButton_perso->isChecked())
        infos.insert("titre", ui->lineEdit_titre_interro->text());
    else{
        QMessageBox::warning(this, "Pas de titre", "Le titre de l'interrogation n'a pu être déterminé");
        return;
    }

    QDate date;
    if(ui->radioButton_date->isChecked())
        infos.insert("date", date.currentDate().toString("dd/MM/yyyy"));
    if(ui->radioButton_date_perso->isChecked())
        infos.insert("date", ui->dateEdit->text());

    QString nomFichier;
    if(ui->radioButton_pdf->isChecked()){
        nomFichier = QFileDialog::getSaveFileName(this, "Enregistre l'interrogation", QDir::homePath(), "Fichiers PDF (*.pdf)");
        if(nomFichier.isEmpty())
            return;
        if(!nomFichier.endsWith(".pdf"))
            nomFichier.append(".pdf");
    }
    if(ui->radioButton_print->isChecked())
        nomFichier = "PRINT!";
    infos.insert("print", nomFichier);

    //Options
    if(ui->checkBox_notes->isChecked()){
        infos.insert("notes", "true");
    }
    if(ui->checkBox_lignes_reponses->isChecked()){
        infos.insert("lignes", "true");
    }

    Pdf myPdf;
    myPdf.generate(questions, infos, false);
    if(prof){
        QString nv_nom = infos.value("print");
        nv_nom.resize(nv_nom.size()-4);
        nv_nom.append("_prof.pdf");
        infos.replace("print", nv_nom);
        myPdf.generate(questions, infos, true);
    }

    //Message de fin
    QMessageBox::information(this, "Questionnaire généré", "Le questionnaire a été généré et se trouve ici:"+nomFichier);
    return;
}

void Interrogator::about(){
    QString string_about = ("<h2>À propos d'Interrogator</h2><br><b>Dévoloppé par</b> : David Lumaye<br><b>Version</b> : 0.0.1<br><b>Courriel</b>:<a href='mailto:littletiger58.aro-base.gmail.com'>littletiger58.aro-base.gmail.com</a><br><b>Distribué sous license</b> : <a href='http://www.gnu.org/licenses/gpl-3.0.fr.html'>GPL 3</a>");
    QMessageBox::about(this, tr("À propos d'Interrogator"), string_about);
    return;
}

void Interrogator::restart(){
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    qApp->quit();
}

void Interrogator::changeStatusAnswer(){
    insManager->setSettings(EmptyAnswers, ui->actionEmptyAnswers->isChecked());
    return;
}

void Interrogator::changeStatusQuestion(){
    insManager->setSettings(EmptyQCMQuestion, ui->actionEmptyQuestions->isChecked());
    return;
}

void Interrogator::setFullSecurity(){
    insManager->setSettings(FullSecurity, ui->actionFullSecurity->isChecked());
    return;
}
