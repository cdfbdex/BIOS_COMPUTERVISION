#ifndef ENTRENAMIENTOMODELOS_H
#define ENTRENAMIENTOMODELOS_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class EntrenamientoModelos;
}

class EntrenamientoModelos : public QWidget
{
    Q_OBJECT

public:
    explicit EntrenamientoModelos(QWidget *parent = 0);
    ~EntrenamientoModelos();

private:
    void connectSignalSlot();
    void initializeObjects();

protected:
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

signals:
    closeWindow();

private slots:
    // SLOTS DE OBJETOS DEL GUI
    void on_pushButton_clicked();

private:
    Ui::EntrenamientoModelos *ui;
};

#endif // ENTRENAMIENTOMODELOS_H
