#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
    names = new QString[1];
    settings = new QVariant[1];

    names[Base] = "FichierBase";

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
    return;
}

void SettingsManager::update(){
    this->loadSettings();
    return;
}

