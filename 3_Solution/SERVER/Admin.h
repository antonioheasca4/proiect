#pragma once
#include<string>
#include <iostream>


class Admin {
private:
    std::string username;
    std::string parola;
    static Admin* instance;

    Admin() 
    { username = "admin"; parola = "admin"; }
    ~Admin() {}
    Admin(const Admin&) = delete;
    Admin(Admin&&) = delete;
    Admin& operator= (const Admin&) = delete;
    Admin&& operator= (Admin&&) = delete;
public:
    bool autentificare(const std::string& user, const std::string& pass);                                                                                                                                                                                                                                                                                                                                               
    bool getCredentiale();

    static Admin* getInstance();
    static void destroyInstance();

    void deleteUser();
    void insertUser();
};
