#ifndef STUDENT_H
#define STUDENT_H

#include "user.h"
#include "homeworks.h"

class Student : public User
{
public:
    Student(QString userName,QString type) : User(userName, type) {}

private:
};

#endif // STUDENT_H
