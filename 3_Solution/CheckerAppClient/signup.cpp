#include <QMessageBox>
#include <QString>
#include <regex>
#include <QRegularExpression>
#include "signup.h"
#include "ui_signup.h"
#include "mainwindow.h"
#include "clientconnection.h"
SignUp::SignUp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);
    showMaximized();

    setWindowTitle("CheckerApp");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(ui->groupBox);
    layout->setAlignment(Qt::AlignCenter);

    ui->groupBox->setFixedSize(600, 350);

    setLayout(layout);


    ui->comboBox->addItem("Student");
    ui->comboBox->addItem("Professor");

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));

}

bool isValidEmailSignUp(const QString& email) {

    QRegularExpression regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    return regex.match(email).hasMatch();
}

bool isValidPassword(const QString& FirstPasswors, const QString& SecondPassword)
{
    if(FirstPasswors==SecondPassword)
        return 1;
    else
        return 0;
}

bool isValidPhoneNumber(const QString& phoneNumber)
{
    QRegularExpression regex("07[0-9]{8}");
    return regex.match(phoneNumber).hasMatch();
}
void SignUp::onComboBoxIndexChanged(int index)
{
    QString selectedText = ui->comboBox->itemText(index);
    qDebug() << "Selected item:" << selectedText;
}

void SignUp::on_pushButton_clicked()
{
    QString FirstName, LastName, Email, Password1, Password2, PhoneNumber;
    int Occupation = ui->comboBox->currentIndex();

    FirstName = ui->lineEdit_FirstName->text();
    LastName = ui->lineEdit_LastName->text();
    Email = ui->lineEdit_Email->text();
    Password1 = ui->lineEdit_Password->text();
    Password2 = ui->lineEdit_Password2->text();
    PhoneNumber = ui->lineEdit_PhoneNumber->text();



    if (isValidEmailSignUp(Email) && isValidPassword(Password1, Password2)==1 && isValidPhoneNumber(PhoneNumber))
    {

        QString typeauthentication = "0";

        QByteArray byteFirstName = FirstName.toUtf8();
        QByteArray byteLastName = LastName.toUtf8();
        QByteArray byteEmail = Email.toUtf8();
        QByteArray bytePassword = Password1.toUtf8();
        QByteArray bytePhoneNumber = PhoneNumber.toUtf8();

        char* buff = (char*)malloc(byteFirstName.size() + byteLastName.size() + byteEmail.size() +
                                     bytePassword.size() + bytePhoneNumber.size() + 6);


        strcpy(buff, typeauthentication.toUtf8().constData());
        strcat(buff, "$");
        strcat(buff, byteFirstName.constData());
        strcat(buff, "$");
        strcat(buff, byteLastName.constData());
        strcat(buff, "$");
        strcat(buff, byteEmail.constData());
        strcat(buff, "$");
        strcat(buff, bytePassword.constData());
        strcat(buff, "$");
        strcat(buff, bytePhoneNumber.constData());
        strcat(buff, "$");
        strcat(buff, QString::number(Occupation).toUtf8().constData());


        if (ClientConnection::getInstance().sendData(buff))
        {

            qDebug() << "Message sent: " << buff;
            this->show();
            Logger::getInstance()->log("Message sent: " + (std::string)buff + "\n");
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        }
        free(buff);
    }
    else
    {
        if (!isValidEmailSignUp(Email))
        {
            QMessageBox::critical(this, "Error", "The email address is not valid.");
            this->show();
            return;
        }
        else if (isValidPassword(Password1, Password2)==0)
        {
            QMessageBox::critical(this, "Error", "Passwords do not match.");
            this->show();
            return;
        }
        else if (!isValidPhoneNumber(PhoneNumber))
        {
            QMessageBox::critical(this, "Error", "The Phone number is not valid.");
            this->show();
            return;
        }
    }


    if( ClientConnection::getInstance().receiveData())
    {
        this->hide();
        this->hide();
        MainWindow* mainWindow = new MainWindow(this);
        mainWindow->show();
    }
    else
    {
        QMessageBox::critical(this, "Eroare", "The email address alredy exists.");
    }
}

void SignUp::on_pushButton_2_clicked()
{
    this->hide();
    MainWindow* mainWindow = new MainWindow(this);
    mainWindow->show();
}

SignUp::~SignUp()
{
    delete ui;
}
