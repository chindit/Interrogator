#include "pdf.h"

Pdf::Pdf(QObject *parent) : QObject(parent){
}

void Pdf::generate(QList<QMultiMap<QString,QString> > questions, QMultiMap<QString,QString> infos, bool prof){
    srand(unsigned(time(NULL)));
    QPrinter printer;
    QPrintDialog *dlg;
    if(infos.value("print") != "PRINT!"){
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(infos.value("print"));
    }
    else{
        dlg = new QPrintDialog(&printer, 0);
        dlg->exec();
        QString temp = printer.printerName();
        printer.setOutputFormat(QPrinter::NativeFormat);
        printer.setOutputFileName(QDir::tempPath()+"/interrogator.ps");
    }
    printer.setPaperSize(QPrinter::A4);
    printer.setPageMargins(75, 100, 75, 100, QPrinter::DevicePixel);
    QPainter page;
    if (!page.begin(&printer))
            return;
    QFont date("Arial", 10);
    QFont titre("Times New Roman", 24, QFont::DemiBold);
    QFont regular("Times New Roman", 12);
    QFont italique("Times New Roman", 12, QFont::StyleItalic);
    QFont large("Times New Roman", 16);
    titre.setUnderline(true);

    bool notes = false, lignes = false;
    int totalNotes = 0;
    //Vérification des notes
    if(infos.value("notes", "false") == "true"){
        notes = true;
        for(int i=0; i<questions.size(); i++){
            if(questions.at(i).value("ponctuation").toInt() <= 0){
                notes = false;
            }
            else
                totalNotes += questions.at(i).value("ponctuation").toInt();
        }

        if(notes){
            //On affiche le total
            page.setFont(large);
            page.drawText(printer.width()-50, 10, "/"+QString::number(totalNotes));
        }
    }

    //lignes
    if(infos.value("lignes", "false") == "true")
        lignes = true;

    //Date
    if(!infos.value("date", "").isEmpty()){
        page.setFont(date);
        page.drawText(printer.width()-30, -60, infos.value("date"));
    }

    page.setFont(titre);
    int totalWidth = printer.width();
    int totalHeight = printer.height();
    QFontMetrics metrics(titre);
    QFontMetrics metricsItalic(italique);
    QFontMetrics metrics2(regular);
    int lineHeight = metrics2.boundingRect("blah").height();
    float hauteurBloc;
    int nbReturn = infos.value("titre").count("\n");
    int nbPages = 1;

    //Titre
    QRect placeTitre = metrics.boundingRect(infos.value("titre"));
    int lineHeightTitre = placeTitre.height();
    hauteurBloc = ceil((float)placeTitre.width()/(float)totalWidth);
    page.drawText(0, 0, totalWidth,((int)hauteurBloc+nbReturn)*lineHeightTitre, Qt::AlignHCenter|Qt::TextDontClip|Qt::TextWordWrap, infos.value("titre"));
    int currentHeight = (((int)hauteurBloc+nbReturn)*lineHeightTitre); //Hauteur du titre

    if(!infos.value("sstitre", "").isEmpty()){
        QFont ssTitre("Ubuntu", 18, QFont::DemiBold);
        page.setFont(ssTitre);
        QFontMetrics metrics3(ssTitre);
        QRect place = metrics3.boundingRect(infos.value("sstitre"));
        lineHeightTitre = place.height();
        hauteurBloc = ceil((float)place.width()/(float)totalWidth);
        nbReturn = infos.value("sstitre").count("\n");
        page.drawText(0, currentHeight, totalWidth, ((int)hauteurBloc+nbReturn)*lineHeightTitre, Qt::AlignHCenter|Qt::TextDontClip|Qt::TextWordWrap, infos.value("sstitre"));
        currentHeight += (((int)hauteurBloc+nbReturn)*lineHeightTitre)+lineHeightTitre;
    }
    else{
        currentHeight += lineHeightTitre; //Espace pour séparer le titre du texte de l'interro
    }

    page.setFont(regular);

    for(int i=0; i<questions.size(); i++){
        QString pregunta = QString::number(i+1)+") "+questions.at(i).value("intitule");
        placeTitre = metrics2.boundingRect(pregunta);
        hauteurBloc = ceil((float)placeTitre.width()/(float)totalWidth); //ceil() sert à arrondir à l'unité supérieure
        //Changement de page
        if(currentHeight+(hauteurBloc*lineHeight) > totalHeight){
            //On indique le numéro de page
            page.setFont(date);
            page.drawText(printer.width(), printer.height()+60, QString::number(nbPages));
            printer.newPage();
            nbPages++;
            page.setFont(regular);
            currentHeight = 0;
        }

        nbReturn = pregunta.count("\n");
        //Intitulé de la question
        page.drawText(0, currentHeight, totalWidth, (((int)hauteurBloc+nbReturn)*lineHeight), Qt::AlignJustify|Qt::TextDontClip|Qt::TextWordWrap, pregunta);
        if(notes){
            //On affiche les notes
            page.drawText(totalWidth+15, currentHeight, "/"+questions.at(i).value("ponctuation"));
        }
        currentHeight += ((int)hauteurBloc+nbReturn+1)*lineHeight;

        //Réponse normale
        if(questions.at(i).value("qcm") == "false" && questions.at(i).value("lignes", "5").toInt() != 0 && !prof && lignes){
            QString points = "...";
            int widthPoints = metrics2.boundingRect(points).width();
            bool completed = false;
            int posWidth = 0; int nbLines = 0;
            while(!completed){
                if(posWidth+widthPoints <= totalWidth){
                    posWidth += widthPoints;
                    page.drawText(posWidth, currentHeight, points);
                }
                else if(posWidth+widthPoints > totalWidth){
                    if(nbLines == (questions.at(i).value("lignes", "5").toInt()-1)){
                        completed = true;
                    }
                    else{
                        nbLines++;
                        posWidth = 0;
                        if(currentHeight+lineHeight > totalHeight){
                            //On indique le numéro de page
                            page.setFont(date);
                            page.drawText(printer.width(), printer.height()+60, QString::number(nbPages));
                            printer.newPage();
                            nbPages++;
                            page.setFont(regular);
                            currentHeight = 0;
                        }
                        else
                            currentHeight += lineHeight;
                    }
                }
            }
        }

        //Éléments de réponses pour une question normale
        if(prof && questions.at(i).value("qcm") == "false"){
            QRect content = metricsItalic.boundingRect(questions.at(i).value("reponse"));
            hauteurBloc = ceil((float)content.width()/(float)totalWidth);
            nbReturn = infos.value("reponse").count("\n");
            page.setFont(italique);
            //Changement de page
            if(currentHeight+((hauteurBloc+nbReturn)*lineHeight) > totalHeight){
                int heightDispo = totalHeight-currentHeight;
                QStringList listeMots = questions.at(i).value("reponse").split(" ");
                QString nouvelleChaine;
                bool depassed = false;
                while(!depassed){
                    QRect size = metricsItalic.boundingRect(nouvelleChaine+listeMots.first()+" ");
                    hauteurBloc = ceil((float)size.width()/(float)totalWidth);
                    hauteurBloc += nouvelleChaine.count("\n");
                    if((hauteurBloc*lineHeight) > heightDispo){
                        depassed = true;
                        page.drawText(0, currentHeight, (totalWidth), ((int)hauteurBloc*lineHeight), Qt::AlignJustify|Qt::TextDontClip|Qt::TextWordWrap, nouvelleChaine);
                    }
                    else{
                        nouvelleChaine.append(listeMots.first()+" ");
                        listeMots.removeFirst();
                    }
                }
                //On indique le numéro de page
                page.setFont(date);
                page.drawText(printer.width(), printer.height()+60, QString::number(nbPages));
                printer.newPage();
                nbPages++;
                page.setFont(regular);
                currentHeight = 0;
                //On écrit le reste du text
                nouvelleChaine.clear();
                for(int i=0; i<listeMots.size(); i++){
                    nouvelleChaine.append(listeMots.at(i)+" ");
                }
                QRect repRect = metricsItalic.boundingRect(pregunta);
                hauteurBloc = ceil((float)repRect.width()/(float)totalWidth);
                page.drawText(0, currentHeight, (totalWidth), ((int)hauteurBloc*lineHeight), Qt::AlignJustify|Qt::TextDontClip|Qt::TextWordWrap, nouvelleChaine);
                nbReturn = nouvelleChaine.count("\n");
                currentHeight += ((int)hauteurBloc+nbReturn+2)*lineHeight;

            }
            else{
                page.drawText(0, currentHeight, (totalWidth), ((int)hauteurBloc*lineHeight), Qt::AlignJustify|Qt::TextDontClip|Qt::TextWordWrap, questions.at(i).value("reponse"));
                currentHeight += ((hauteurBloc+nbReturn)*lineHeight);
            }
            page.setFont(regular);
        }

        //QCM
        if(questions.at(i).value("qcm") == "true"){
            QChar carre(0x25A2); //Carré vide à bords arrondis
            QChar carre2(0x25A3); //Carré rempli
            QMultiMap<QString, QString> mapQuestion = questions.at(i);
            QMapIterator<QString, QString> it(mapQuestion);
            QList < int > reponsesCorrectes;
            int nbReponses = 0;
            while(it.hasNext()){
                it.next();
                if(it.key() == "valide")
                    reponsesCorrectes.append(QString(it.value()).toInt());
                bool convert = false;
                QString(it.key()).toInt(&convert);
                if(convert)
                    nbReponses++;
            }

            QList<int> tires;
            std::list<int> myListShuffled;
            for(int i=0; i<nbReponses; ++i){
                bool isOk = false;
                while(!isOk){
                    int resultat = rand() % nbReponses + 1;
                    if(!tires.contains(resultat)){
                        tires.append(resultat);
                        isOk = true;
                        myListShuffled.push_back(resultat);
                    }
                }
            }
            std::list<int>::const_iterator iterator;
            for (iterator = myListShuffled.begin(); iterator != myListShuffled.end(); ++iterator){
                int id=*iterator;
                    QString texte;
                    if(prof && reponsesCorrectes.contains(id))
                        texte = QString(carre2);
                    else
                        texte = QString(carre);
                    texte += " "+mapQuestion.value(QString::number(id));
                    float hauteurBloc = ceil((float)metricsItalic.boundingRect(texte).width()/(float)totalWidth);
                    //Changement de page
                    if(currentHeight+(hauteurBloc*lineHeight) > totalHeight){
                        //On indique le numéro de page
                        page.setFont(date);
                        page.drawText(printer.width(), printer.height()+60, QString::number(nbPages));
                        printer.newPage();
                        nbPages++;
                        page.setFont(regular);
                        currentHeight = 0;
                    }
                    page.drawText(0, currentHeight, (totalWidth), ((int)hauteurBloc*lineHeight), Qt::AlignJustify|Qt::TextDontClip|Qt::TextWordWrap, texte);
                    currentHeight += (int)hauteurBloc*lineHeight;
                //}
            }
        }
        currentHeight += 2*lineHeight; //Double espace pour bien marquer la séparation entre question et réponse.
    }

    //On affiche le numéro de la dernière page
    page.setFont(date);
    page.drawText(printer.width(), printer.height()+60, QString::number(nbPages));
    page.end();

    //Impression réelle
    /*if(infos.value("print") == "PRINT!"){
        dlg = new QPrintDialog(&printer, 0);
        dlg->setWindowTitle(tr("Imprimer"));
        if (dlg->exec() != QDialog::Accepted)
            return;
    }*/
    return;
}
