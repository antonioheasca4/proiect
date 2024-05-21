#pragma once
#include "Utilitati.h"
#include "Logger.h"
#include <winsock2.h>
#include <string>
#include <vector>


class IAction
{
public:
	virtual void manageAction() = 0;
	virtual void sendRaspuns(SOCKET s) = 0;
};

class FactoryActions
{
public:
	static IAction* registerAction(std::vector<std::wstring> date);
	static IAction* loginAction(std::vector<std::wstring> date);
};