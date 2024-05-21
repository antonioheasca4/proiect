#include "Register.h"
#include "DB.h"

Register::Register(std::vector<std::wstring> date)
{
	this->nume = date[1];
	this->prenume = date[2];
	this->email = date[3];
	this->parola = date[4];
	this->nrTel = date[5];
	this->ocupatie = std::stoi(date[6]);

	this->raspuns = "";
}

void Register::manageAction()
{
	if (DB::getInstance()->insertUser(this->nume, this->prenume, this->email, this->parola, this->nrTel, this->ocupatie))
	{
		this->raspuns = "ok";
		Logger::getInstance()->log("Utilizator inregistrat cu succes!!!\n");
	}
	else
	{
		this->raspuns = "no";
		Logger::getInstance()->log("Nu s-a putut realiza inregistrarea!!!\n");
	}
}


