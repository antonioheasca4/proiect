#pragma once
#include "IAction.h"

class AAction : public IAction
{
protected:
	std::string raspuns;
	void sendRaspuns(SOCKET s) override;
};

