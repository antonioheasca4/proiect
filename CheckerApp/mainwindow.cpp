#define _CRT_SECURE_NO_WARNINGS
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QHBoxLayout>
#include <QScreen>
#include <QMessageBox>
#include <regex>
#include "utils.h"
#include <QRegularExpression>
#include "clientconnection.h"
#include "signup.h"
#include "iuser.h"
#include "homewindowb.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    if (ClientConnection::getInstance().connectToServer("172.20.10.2", 12345))
    {
        ui->setupUi(this);
        showMaximized();
        setWindowTitle("CheckerApp");
       // QPixmap pix(":/resources/image/imageSignUp.jpg");
       // ui->label->setPixmap(pix);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    char* buff;
    QString Email, Password;
    QString typeauthentication = "1";
    Email = ui->lineEdit_Username->text();
    Password = ui->lineEdit_Password->text();

    if(!Email.isEmpty() && !Password.isEmpty())
    {
        QByteArray byteEmail = Email.toUtf8();
        QByteArray bytePass = Password.toUtf8();

        const char* dataEmail = byteEmail.constData();
        const char* dataPassword = bytePass.constData();

        buff= (char*)malloc(strlen(dataEmail) + strlen(dataPassword) +3);
        strcpy(buff, typeauthentication.toUtf8().constData());
        strcat(buff,"$");
        strcat(buff,dataEmail);
        strcat(buff,"$");
        strcat(buff,dataPassword);

        if (ClientConnection::getInstance().sendData(buff))
        {
            QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
            qDebug() << message;
            Logger::getInstance()->log(message.toStdString());
        }

        if(ClientConnection::getInstance().receiveData())
        {
            if (ClientConnection::getInstance().getTypeAuhhentification()=='0')
            {
                QString username = ui->lineEdit_Username->text();
                IUser* currentUser=FactoryUsers::createStudent(username, "Student");
                this->hide();
                HomeWindowB* homeStudentWindow = new HomeWindowB(this, currentUser);
                homeStudentWindow->show();
            }

            if (ClientConnection::getInstance().getTypeAuhhentification()=='1')
            {
                QString username = ui->lineEdit_Username->text();
                IUser *currentUser=FactoryUsers::createProfessor(username, "Professor");
                this->hide();
                HomeWindowB* homeStudentWindow = new HomeWindowB(this, currentUser);
                homeStudentWindow->show();
            }
        }
        else
        {
            QMessageBox::critical(this, "Eroare", "The email address or password is not valid.");
        }
        if(buff)
            free(buff);
    }
    else
    {
        QMessageBox::critical(this, "Eroare", "The email address or password are empty.");
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    SignUp* signUpWindow = new SignUp(this);
    signUpWindow->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

