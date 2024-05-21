#pragma once
#include "AExceptie.h"

class CompilerExceptie : public AExceptie
{
	std::string fileName;
public:
	CompilerExceptie(const std::string str,const std::string& nume) : AExceptie(str),fileName(nume) { this->setTipEroare(comp);};
	void handle() override;
};

