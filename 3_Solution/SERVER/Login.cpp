#include "Login.h"
#include "DB.h"

Login::Login(std::vector<std::wstring> date)
{
	this->email = date[1];
	this->parola = date[2];
	this->ocupatie = 0;
}

void Login::manageAction()
{
	if (DB::getInstance()->checkLogin(this->email, this->parola))
	{
		this->ocupatie = DB::getInstance()->getOcupatie(this->email, this->parola);
		this->raspuns = "ok#" + std::to_string(this->ocupatie);
		Logger::getInstance()->log("Utilizator logat cu succes!!!\n");
	}
	else
	{
		this->raspuns = "no\n";
		Logger::getInstance()->log("Utilizatorul nu se poate loga.Nu exista acest utilizator in baza de date!!!\n");
	}
}

