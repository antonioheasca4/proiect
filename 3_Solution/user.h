#ifndef USER_H
#define USER_H

#include "iuser.h"
#include "homeworks.h"

class User : public IUser
{
public:
    User(QString userName, QString type) : IUser() {
        this->userName=userName;
        this->type=type;
    }
    const QString getUserName() {return userName;}
    void setHomeworks();
    std::vector<Homeworks> getHomeworks() { setHomeworks();
        return homeworks;}
    std::vector<Homeworks> getHomeworks2() { return homeworks;}
    void setHomeworksEmpty()
    {
        homeworks.clear();
    }
    const QString getType() {return this->type;}

private:
    std::vector<Homeworks> homeworks;
    QString userName;
    QString type;
};

#endif // USER_H
