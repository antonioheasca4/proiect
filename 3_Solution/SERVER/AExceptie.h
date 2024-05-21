#pragma once
#include "IExceptie.h"

typedef enum tipEroare
{
	BD,comp
};

class AExceptie : public IExceptie
{
private:
	std::string mesajEroare;
	tipEroare errorType;
public:
	AExceptie(const std::string s) : mesajEroare(s) { errorType = BD;} //DEFAULT BD (BAZA DATE)
	const std::string& getMesajEroare() { return mesajEroare; }
	void setTipEroare(tipEroare t) { this->errorType = t; }
	const tipEroare getTipEroare() { return errorType; }
};

