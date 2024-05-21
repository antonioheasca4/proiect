#include "user.h"
#include "clientconnection.h"
#include "Logger.h"
#include <QDebug>


Homeworks parseHomework(const std::string& line) {
    std::istringstream iss(line);
    std::string title, description, professor, upload;

    // Citim informațiile separate de caracterul '$'
    std::getline(iss, title, '$');
    std::getline(iss, description, '$');
    std::getline(iss, professor, '$');
    std::getline(iss, upload, '$');
    int upload1=stoi(upload);

    return Homeworks(title, description, professor, upload1);
}

void User::setHomeworks()
{
    QString username=this->getUserName();
    QByteArray usernameCode = username.toUtf8();
    const char* usernameCode2 = usernameCode.constData();


    char * buffer=(char*)malloc(this->getUserName().length()+3);

    if(buffer)
    {
        strcpy(buffer, "H$");
        strcat(buffer, usernameCode2);
    }

    if (ClientConnection::getInstance().sendData(buffer)) {
        QString message = "Message sent: " + QString::fromUtf8(buffer) + "\n";
        qDebug() << message;
        Logger::getInstance()->log(message.toStdString());
    }

    std::string numberHomeworksS=ClientConnection::getInstance().receiveDataHomeworks();
    int numberHomeworks=stoi(numberHomeworksS);

    if (ClientConnection::getInstance().sendData("MORE"))
    {
        QString message = "Message sent: " + QString::fromUtf8("MORE") + "\n";
        qDebug() << message;
        Logger::getInstance()->log(message.toStdString());
    }

    std::string homeworksAll;

    if (numberHomeworks == 0) {
        Homeworks home=Homeworks("NULL", "NULL", "NULL", 0);
        this->homeworks.push_back(home);
    }
    else{
        for(int i=0; i<numberHomeworks; i++){
            homeworksAll = ClientConnection::getInstance().receiveDataHomeworks();
            Homeworks home=parseHomework(homeworksAll);
            this->homeworks.push_back(home);
            if (ClientConnection::getInstance().sendData("MORE"))
            {
                QString message = "Message sent: " + QString::fromUtf8("MORE") + "\n";
                qDebug() << message;
                Logger::getInstance()->log(message.toStdString());
            }
        }
    }
}
