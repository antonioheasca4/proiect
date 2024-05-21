#ifndef IUSER_H
#define IUSER_H
#include <QString>

#include "homeworks.h"

enum UserType {
    StudentType,
    ProfessorType,
    AdminType
};

class IUser
{
public:
    IUser(){}
    virtual const QString getUserName()=0;
    virtual std::vector<Homeworks> getHomeworks() = 0;
    virtual std::vector<Homeworks> getHomeworks2() = 0;
    virtual const QString getType() =0;
    virtual void setHomeworksEmpty() =0;
private:

};

class FactoryUsers
{
public:
    static IUser* createStudent(QString userName,QString type);
    static IUser* createProfessor(QString userName,QString type);
    static IUser* createAdmin(QString userName,QString type);
};

#endif // IUSER_H



