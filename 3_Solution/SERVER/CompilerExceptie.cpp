#include "CompilerExceptie.h"

void CompilerExceptie::handle()
{
	std::cerr << this->getMesajEroare() + this->fileName << std::endl;
	Logger::getInstance()->log("Eroare la dechiderea fisierului " + this->fileName + "\n");
}
