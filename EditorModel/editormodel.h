#ifndef EDITORMODEL_H
#define EDITORMODEL_H

#include <QtGui/QMainWindow>
#include "ui_editormodel.h"

class EditorModel : public QMainWindow
{
    Q_OBJECT

public:
    EditorModel(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~EditorModel();

private:
    Ui::EditorModelClass ui;
};

#endif // EDITORMODEL_H
