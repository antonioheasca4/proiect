#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"

class Admin : public User
{
public:
    Admin(QString userName,QString type) : User(userName, type) {}
    std::vector<Homeworks> getHomeworks() {}
};

#endif // ADMIN_H
