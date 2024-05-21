#include "Connection.h"
#include "IExceptie.h"


int main(int argc, char* argv[])
{
	try
	{
		Connection::getInstance()->startConnection(12345);
		Connection::destroyInstance();
	}
	catch(IExceptie* e)
	{
		e->handle();
		delete e;
	}
	return 0; 
}