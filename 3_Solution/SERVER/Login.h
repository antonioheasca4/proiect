#pragma once
#include "AAction.h"

class Login : public AAction
{
	std::wstring email;
	std::wstring parola;
	int ocupatie;
public:
	Login(std::vector<std::wstring> date);
	void manageAction() override;
};

