#ifndef INTERROGATOR_H
#define INTERROGATOR_H

#include <QDate>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QSignalMapper>
#include <QVector>
#include <time.h>

#include "binder/bindermodel.h"
#include "binder/bindereditiondialog.h"
#include "pdf.h"
#include "settingsmanager.h"
#include "toolsdialog.h"
#include "xml.h"

namespace Ui {
class Interrogator;
}

class Interrogator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Interrogator(QWidget *parent = 0);
    ~Interrogator();

private slots:
    void changeStacked(QString widget);
    void saveCateg();
    void updateList();
    void categoryActivated(int row, int col);
    void categoryActivatedQuestion(int row, int col);
    void setQCMStatus();
    void addQCMAnswer();
    void saveQuestion();
    void setCategory(int category);
    void setQuestion(int question, int categorie);
    void preparePDF();
    void about();
    void restart();
    void changeStatusAnswer();
    void changeStatusQuestion();
    void setFullSecurity();
    void undoQuestion();
    void undoCateg();
    void binderSelectionUpdated(QModelIndex selectedIndex);
    void processBinderEdition(QMap<QString, QString> binder);
    void editBinder();

private:
    void buildUI();
    void setListCateg();
    void updateListCateg();

    BinderModel *binderTree;
    BinderEditionDialog *binderEditionDialog;
    QModelIndex currentSelectedIndex;

    Ui::Interrogator *ui;
    Xml *insXml;
    SettingsManager *insManager;
    ToolsDialog *insDialog;
    bool edit;
    int idEdit;
    int idEditCateg;
};

#endif // INTERROGATOR_H