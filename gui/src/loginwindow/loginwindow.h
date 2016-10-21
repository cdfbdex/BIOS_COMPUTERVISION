#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMessageBox>

#include <MABE_core>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

signals:
    void inicioSesion_Success(QString userName, int permissionLevel);
    void cerrarSesion_Success();
    void changeLanguage_Triggered();

private:
    void connectSignalSlot();
    void initializeObjects();
    void validateCredentialsLogin(QByteArray &response);
    void validatePrivilegesOfUser(QByteArray &response);

protected:
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    // SLOTS DEL WEBSERVICE
    void handleResponse(QByteArray response);
    void handleNetworkError(QString errorString);

    // SLOTS DE OBJETOS DEL GUI
    void validateUserTextInput(QString text);
    void validarCredenciales_onClicked();
    void cancelar_onClicked();

private:
    Ui::LoginWindow *ui;

    bool loggedIn = false;

    QString lastUserLoggedIn = "NULL";
    QString lastPassLoggedIn = "NULL";
    int LastWebServiceRequested = RQ_NULL;
    APIController *m_apiControl;
    QByteArray serverResponse;
};

#endif // LOGINWINDOW_H
