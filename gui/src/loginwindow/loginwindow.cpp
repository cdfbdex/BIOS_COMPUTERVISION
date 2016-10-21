#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    initializeObjects();

    connectSignalSlot();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::initializeObjects()
{
    // Inicialización de Variables


    // Gestión de webservices
    m_apiControl = new APIController(this);

    // Formato para mensaje para validar datos de entrada
    ui->label_verificacionUsuario->setVisible(false);
    ui->label_verificacionUsuario->setStyleSheet("background-color: #FE2E2E;color:white;");
}

void LoginWindow::connectSignalSlot()
{
    // SIGNALS Y SLOTS PARA CONEXIÓN CON WEBSERVICE
    connect(m_apiControl,SIGNAL(dataRequestedReady(QByteArray)),this,SLOT(handleResponse(QByteArray)));
    connect(m_apiControl,SIGNAL(networkError(QString)),this,SLOT(handleNetworkError(QString)));

    // SIGNALS Y SLOTS PARA OBJETOS DEL GUI
    connect(ui->lineEdit_usuario,SIGNAL(textChanged(QString)),this,SLOT(validateUserTextInput(QString)));
    connect(ui->lineEdit_usuario,SIGNAL(returnPressed()),ui->lineEdit_contrasena,SLOT(setFocus()));
    connect(ui->lineEdit_contrasena,SIGNAL(returnPressed()),this,SLOT(validarCredenciales_onClicked()));

    connect(ui->pushButton_Enviar,SIGNAL(clicked()),this,SLOT(validarCredenciales_onClicked()));
    connect(ui->pushButton_Cancelar,SIGNAL(clicked()),this,SLOT(cancelar_onClicked()));
}


void LoginWindow::validateCredentialsLogin(QByteArray &response)
{
    /// TODO: Parse respuesta del servicio Login
    if(response != "null")
    {
        // Extracción de valores del JSON
        QJsonObject jsonOriginal = JsonController::readJSONFromByteArray(response);
        QJsonObject jsonReceived = jsonOriginal["autenticarUsuarioResult"].toObject();

        qDebug() << "TEST JSON OBJ: " << jsonReceived;

        QString idOperador = jsonReceived["id_operador"].toString();
        int privilegeLevel = jsonReceived["perfil"].toString().toInt();

        qDebug() << "Inicio de sesión - Operador:" << idOperador << " - Privilegios: " << privilegeLevel;

        if(!loggedIn) // Iniciar sesión
        {
            // Iniciar sesión con Nombre de usuario y Nivel de privilegios
            // PERMISOS: 1-ADMIN 2-JEFE CALIDAD 3-OPERARIO
            emit inicioSesion_Success(idOperador,privilegeLevel);
            lastUserLoggedIn = ui->lineEdit_usuario->text();
            lastPassLoggedIn = ui->lineEdit_contrasena->text();
            loggedIn = true;
            this->hide();
        }
        else
        {
            qDebug() << "El usuario ya ha iniciado sesión previamente";
        }
    }
    else
    {
        QMessageBox::information(this,tr("Error en las credenciales"),
                                tr("No puede iniciar sesión, por favor verifique usuario y contraseña."));
    }
    // Limpiar valores en cada validación
    ui->lineEdit_usuario->setText("");
    ui->lineEdit_contrasena->setText("");
    ui->lineEdit_usuario->setFocus();
}

void LoginWindow::validatePrivilegesOfUser(QByteArray &response)
{
    /// TODO: Parse respuesta del servicio Logout
    if(response != "null")
    {
        // Extracción de valores del JSON
        QJsonObject jsonOriginal = JsonController::readJSONFromByteArray(response);
        QJsonObject jsonReceived = jsonOriginal["cerrarSesionResult"].toObject();

        int privilegeLevel = jsonReceived["perfil"].toString().toInt();
        bool privilegesToLogout = (privilegeLevel==1 || privilegeLevel==2);

        qDebug() << "JSON cierre de sesión: " << privilegesToLogout;

        if(loggedIn && privilegesToLogout) // Cerrar sesión
        {
            emit cerrarSesion_Success();
            lastUserLoggedIn = "NULL";
            lastPassLoggedIn = "NULL";
            loggedIn = false;
            this->hide();
        }
        else
        {
            QMessageBox::information(this,tr("Error en las credenciales"),
                                    tr("No puede cerrar sesión, por favor verifique usuario y contraseña."));
        }
    }
    else
    {
        qDebug() << "El archivo JSON obtenido tiene valor null";
    }
    // Limpiar valores en cada validación
    ui->lineEdit_usuario->setText("");
    ui->lineEdit_contrasena->setText("");
    ui->lineEdit_usuario->setFocus();
}

void LoginWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QMainWindow::changeEvent(event);
}

void LoginWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

///// SLOTS /////
void LoginWindow::handleResponse(QByteArray response)
{
    serverResponse = response;
    qDebug() << "RESPUESTA SERVER: " << serverResponse;

    // Verificar cual fue el servicio solicitado
    switch (LastWebServiceRequested)
    {
    case RQ_LOGIN: // Inicio de sesión
        validateCredentialsLogin(serverResponse);
        break;
    case RQ_LOGOUT: // Cerrar sesión
        validatePrivilegesOfUser(serverResponse);
        break;
    default:
        break;
    }
    LastWebServiceRequested = RQ_NULL;
}

void LoginWindow::handleNetworkError(QString errorString)
{
    QMessageBox::information(0,tr("Error de red"),tr("Error al conectar con el servidor:\n")+errorString);
}

void LoginWindow::validateUserTextInput(QString text)
{
    // Validar si el nombre de usuario son solo dígitos numéricos
    bool allDigits;
    for (int i=0;i<text.length();i++) {
        QString(text.at(i)).toInt(&allDigits);
        if(!allDigits) break;
    }

    // El usuario contiene dígitos no numéricos
    if (!allDigits && text.size()>0)
    {
        ui->lineEdit_usuario->setStyleSheet("background-color: #FE2E2E;color:white;");
        ui->label_verificacionUsuario->setVisible(true);
    }
    else
    {
        ui->lineEdit_usuario->setStyleSheet("");
        ui->label_verificacionUsuario->setVisible(false);
    }
}

void LoginWindow::validarCredenciales_onClicked()
{ 
    /*
    //TODO: ELIMINAR ESTO CUANDO ACABE EL DEBUG
    QString user2 = ui->lineEdit_usuario->text();
    QString pass2 = ui->lineEdit_contrasena->text();

    if(!loggedIn)
    {
        QJsonDocument jsonDoc(JsonController::readJSONFromFile(WS_LOGIN));
        serverResponse = jsonDoc.toJson();
        qDebug() << jsonDoc;
        validateCredentialsLogin(serverResponse);
    }
    else
    {
        bool sameUser = (user2 == lastUserLoggedIn);
        bool samePass = (pass2 == lastPassLoggedIn);

        // Si las credenciales son las mismas que las del usuario loggeado actualmente
        if (sameUser && samePass)
        {
            emit cerrarSesion_Success();
            loggedIn = false;
            lastUserLoggedIn = "NULL";
            lastPassLoggedIn = "NULL";
            ui->lineEdit_usuario->setText("");
            ui->lineEdit_contrasena->setText("");
            this->hide();
        }
        else // Verificar si es un Jefe de calidad o Admin
        {
            QJsonDocument jsonDoc(JsonController::readJSONFromFile(WS_LOGOUT));
            serverResponse = jsonDoc.toJson();
            qDebug() << jsonDoc;
            validatePrivilegesOfUser(serverResponse);
        }
    }
    return;
    */


    //////////////////// WEBSERVICE REAL FUNCIONAL

    QString user = ui->lineEdit_usuario->text();
    QString pass = ui->lineEdit_contrasena->text();

    //Verificar si se está enviando la información para iniciar sesión o cerrar sesión.
    if(!loggedIn)
    {
        // Archivo de configuración del sistema
        QSettings *settings = new QSettings(SETTINGS_FILEPATH,QSettings::IniFormat);

        // Ejecutar método HTTP GET
        qDebug() << WS_LOGIN.arg(user,pass,settings->value("System/assemblyLineID").toString());
        m_apiControl->getRequest(WS_LOGIN.arg(user,pass,QString("L1")/*settings->value("System/assemblyLineID").toString()*/));

        LastWebServiceRequested = RQ_LOGIN;
    }
    else // Cerrar sesión
    {
        bool sameUser = (user == lastUserLoggedIn);
        bool samePass = (pass == lastPassLoggedIn);

        // Si las credenciales son las mismas que las del usuario loggeado actualmente
        if (sameUser && samePass)
        {
            emit cerrarSesion_Success();
            lastUserLoggedIn = "NULL";
            lastPassLoggedIn = "NULL";
            loggedIn = false;

            ui->lineEdit_usuario->setText("");
            ui->lineEdit_contrasena->setText("");
        }
        else // Verificar si es un Jefe de calidad o Admin
        {
            // Ejecutar método HTTP GET
            qDebug() << WS_LOGOUT.arg(user,pass);
            m_apiControl->getRequest(WS_LOGOUT.arg(user,pass));

            LastWebServiceRequested = RQ_LOGOUT;
        }
    }
}

void LoginWindow::cancelar_onClicked()
{
    this->close();
}
