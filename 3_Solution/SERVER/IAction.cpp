#include "IAction.h"
#include "Register.h"
#include "Login.h"

IAction* FactoryActions::registerAction(std::vector<std::wstring> date)
{
    return new Register(date);
}

IAction* FactoryActions::loginAction(std::vector<std::wstring> date)
{
    return new Login(date);
}
