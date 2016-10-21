#include "entrenamientomodelos.h"
#include "ui_entrenamientomodelos.h"

EntrenamientoModelos::EntrenamientoModelos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntrenamientoModelos)
{
    ui->setupUi(this);

    initializeObjects();

    connectSignalSlot();
}

EntrenamientoModelos::~EntrenamientoModelos()
{
    delete ui;
}

void EntrenamientoModelos::connectSignalSlot()
{

}

void EntrenamientoModelos::initializeObjects()
{

}

void EntrenamientoModelos::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

void EntrenamientoModelos::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    event->accept();
}

///// SIGNALS /////
void EntrenamientoModelos::on_pushButton_clicked()
{
    this->close();
}
