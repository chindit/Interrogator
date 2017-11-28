#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
    names = new QString[4];
    settings = new QVariant[4];

    names[Base] = "FichierBase";
    names[EmptyAnswers] = "ReponsesVides";
    names[FullSecurity] = "SauvegardeAutomatique";
    names[EmptyQCMQuestion] = "QuestionQCMVide";

    loadSettings();

}

QVariant SettingsManager::getSettings(Setting s){
    return settings[s];
}

void SettingsManager::setSettings(Setting s, QVariant v){
    if(settings[s] != v){
        settings[s] = v;
        //Enregistrement
        QSettings options("Interrogator", "interrogator");
        options.setValue(names[s], settings[s]);
    }
    else{
        return;
    }
}

void SettingsManager::loadSettings(){
    QSettings options("Interrogator", "interrogator");
    settings[Base] = options.value(names[Base], "");
    settings[EmptyAnswers] = options.value(names[EmptyAnswers], false);
    settings[FullSecurity] = options.value(names[FullSecurity], false);
    settings[EmptyQCMQuestion] = options.value(names[EmptyQCMQuestion], false);
    return;
}

void SettingsManager::update(){
    this->loadSettings();
    return;
}

