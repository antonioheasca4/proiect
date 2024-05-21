#pragma once
#include "AExceptie.h"

class DBExceptie : public AExceptie
{
private:
	const std::string nameServer = "localhost\\ANTONIO";
	const std::string nameDB = "CheckerApp";
public:
	DBExceptie(const std::string s) : AExceptie(s){}
	void handle() override;
};

