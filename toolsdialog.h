#ifndef TOOLSDIALOG_H
#define TOOLSDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "settingsmanager.h"

namespace Ui {
class toolsDialog;
}

class ToolsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolsDialog(QWidget *parent = 0);
    ~ToolsDialog();

private slots:
    void openBase();

signals:
    void baseUpdated();

private:
    Ui::toolsDialog *ui;
};

#endif // TOOLSDIALOG_H
