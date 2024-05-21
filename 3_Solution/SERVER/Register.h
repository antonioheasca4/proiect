#pragma once
#include "AAction.h"

class Register : public AAction
{
	std::wstring nume;
	std::wstring prenume;
	std::wstring email;
	std::wstring parola;
	std::wstring nrTel;
	int ocupatie;

	
public:
	Register(std::vector<std::wstring> date);
	void manageAction() override;
};

