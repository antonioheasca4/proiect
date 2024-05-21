#include "Admin.h"
#include "DB.h"

Admin* Admin::instance = nullptr;

Admin* Admin::getInstance()
{
    if (instance == nullptr)
        instance = new Admin;
    return instance;
}

void Admin::destroyInstance()
{
    if (instance)
    {
        delete instance;
    }
}

void Admin::deleteUser()
{
    std::string email;
    std::cout << "Introdu email: ";
    std::cin >> email;
    std::wstring e(email.begin(), email.end());
    DB::getInstance()->deleteUserByAdmin(e);
}

void Admin::insertUser()
{
    std::string nume;
    std::cout << "Introdu nume: ";
    std::cin >> nume;
    std::string prenume;
    std::cout << "Introdu prenume: ";
    std::cin >> prenume;
    std::string email;
    std::cout << "Introdu email: ";
    std::cin >> email;
    std::string parola;
    std::cout << "Introdu parola: ";
    std::cin >> parola;
    std::string phone;
    std::cout << "Introdu telefon: ";
    std::cin >> phone;

    int ocupatie;
    std::cout << "Introdu ocupatie (0-studenti,1-profesori): ";
    std::cin >> ocupatie;

    std::wstring e(email.begin(), email.end());
    std::wstring n(nume.begin(), nume.end());
    std::wstring p(prenume.begin(), prenume.end());
    std::wstring pass(parola.begin(), parola.end());
    std::wstring t(phone.begin(), phone.end());
    DB::getInstance()->insertUser(n, p, e, pass, t, ocupatie);
}

bool Admin::autentificare(const std::string& user, const std::string& pass)
{
    return (username == user && parola == pass);
}

bool Admin::getCredentiale()
{
        std::string inputUsername;
        std::string inputParola;

        std::cout << "Enter username: ";
        std::cin >> inputUsername;
        std::cout << "Enter password: ";
        std::cin >> inputParola;

        if (this->autentificare(inputUsername, inputParola)) 
        {
            std::cout << "Adminul s-a autentificat cu succes!" << std::endl;
            return 1;
        }
        else 
        {
            std::cout << "Nu esti adminul!!!" << std::endl;
            return 0;
        }
}
