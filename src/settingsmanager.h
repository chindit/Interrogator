#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

enum Setting { Base, EmptyAnswers, EmptyQCMQuestion, FullSecurity };

#include <QObject>
#include <QMessageBox>
#include <QVariant>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = 0);
    QVariant getSettings(Setting s);
    void setSettings(Setting s, QVariant v);

public slots:
    void update();
    
signals:
    
private:
    void loadSettings();

private:
    QVariant *settings;
    QString *names;
    
};

#endif // SETTINGSMANAGER_H
