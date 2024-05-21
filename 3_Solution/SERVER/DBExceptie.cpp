#include "DBExceptie.h"

void DBExceptie::handle()
{
	std::cerr << this->getMesajEroare();
	Logger::getInstance()->log(this->getMesajEroare());
	std::cerr << "Nu se poate conecta la serverul " << this->nameServer << ", baza de date " << this->nameDB;
}
