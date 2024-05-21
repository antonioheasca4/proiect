#ifndef PROFESSOR_H
#define PROFESSOR_H

#include "user.h"

class Professor : public User
{
public:
    Professor(QString userName,QString type) : User(userName, type) {}
};

#endif // PROFESSOR_H
