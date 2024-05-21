#include "homeworks.h"
#include "clientconnection.h"
#include <QString>
#include <QDebug>

HomeworkSolution parseSolution(const std::string& line)
{
    std::istringstream iss(line);
    std::string nameStudent, solution;

    std::getline(iss, nameStudent, '$');
    std::getline(iss, solution, '$');

    return HomeworkSolution(nameStudent, solution);
}


void Homeworks::setSolution(QString nameProfessor)
{
    QString title = QString::fromStdString(this->getTitle());
    QByteArray titleCode = title.toUtf8();
    const char* titleCode2 = titleCode.constData();


    QByteArray nameProfesor2 = nameProfessor.toUtf8();
    const char* nameProfesor3 = nameProfesor2.constData();


    char * buffer=(char*)malloc(this->getTitle().length()+strlen(nameProfesor3)+3);

    if(buffer)
    {
        strcpy(buffer, "R$");
        strcat(buffer, titleCode2);
        strcat(buffer, "$");
        strcat(buffer, nameProfesor3);

        if (ClientConnection::getInstance().sendData(buffer)) {
            QString message = "Message sent: " + QString::fromUtf8("buffer") + "\n";
            qDebug() << message;
            //Logger::getInstance()->log(message.toStdString());
        }

        std::string numberHomeworksR=ClientConnection::getInstance().receiveDataCompilate();
        int numberHomeworksR2=stoi(numberHomeworksR);
        if (ClientConnection::getInstance().sendData("MORE"))
        {
            QString message = "Message sent: " + QString::fromUtf8("MORE") + "\n";
            qDebug() << message;
            //Logger::getInstance()->log(message.toStdString());
        }


        std::string solutionAll;

        if (numberHomeworksR2 == 0)
        {
            this->solution.push_back(parseSolution("NULL$NULL"));
        }
        else{
            for(int i=0; i<numberHomeworksR2; i++){
                solutionAll = ClientConnection::getInstance().receiveDataCompilate();
                HomeworkSolution solution=parseSolution(solutionAll);
                this->solution.push_back(solution);

                if (ClientConnection::getInstance().sendData("MORE"))
                {
                    QString message = "Message sent: " + QString::fromUtf8("MORE") + "\n";
                    qDebug() << message;
                    Logger::getInstance()->log(message.toStdString());
                }
            }
        }
    }
}




