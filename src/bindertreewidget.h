#ifndef BINDERBUILDER_H
#define BINDERBUILDER_H

#include <QObject>
#include <QTreeWidget>

class BinderTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit BinderTreeWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    void enableDragAndDrop();

};

#endif // BINDERBUILDER_H
