#include "AAction.h"

void AAction::sendRaspuns(SOCKET s)
{
	send(s, this->raspuns.c_str(), this->raspuns.size(), 0);
}
