#include "iuser.h"
#include "student.h"
#include "admin.h"
#include "professor.h"

IUser* FactoryUsers::createStudent(QString userName,QString type)
{
    return new Student(userName,type);
}

IUser* FactoryUsers::createProfessor(QString userName,QString type)
{
    return new Professor(userName, type);
}

IUser* FactoryUsers::createAdmin(QString userName,QString type)
{
    return new Admin(userName, type);
}
