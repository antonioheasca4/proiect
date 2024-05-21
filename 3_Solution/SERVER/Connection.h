#pragma once


#include "IAction.h"
#include "DB.h"
#include "Compiler.h"
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>

class Connection
{
private:

	Connection() {}
	~Connection(){}
	Connection(const Connection&) = delete;
	Connection(Connection&&) = delete;
	Connection& operator= (const Connection&) = delete;
	Connection&& operator= (Connection&&) = delete;

	static Connection* instance;


	int serverSocket;

	bool createSocket(int port);
	bool bindSocket(int port);
	bool listenForConnections();
	bool listenForAdmin();
	//bool listenExitAdmin();
	void handleClient(int socket);

	std::string fileName100 = "Cuvinte100.txt";
	std::string fileName = "CuvinteAproape100.txt";

	void loginRegister(std::vector<std::wstring>& wstr, SOCKET s);
	void getHomeworks(std::vector<std::wstring>& wstr, SOCKET s);
	void trimitereTema(std::vector<std::wstring>& wstr, SOCKET s);
	void veziTemplate(std::vector<std::wstring>& wstr, SOCKET s);
	void profesorIncarcaTema(std::vector<std::wstring>& wstr, SOCKET s);
	void incarcareTemplateVectorRezultate(std::vector<std::wstring>& wstr, SOCKET s);
	void veziVectorRezultate(std::vector<std::wstring>& wstr, SOCKET s);
	void veziRezolvariTema(std::vector<std::wstring>& wstr, SOCKET s);
	void procentCheckOutputProf(std::vector<std::wstring>& wstr, SOCKET s);
	void makeRedCommon(std::vector<std::wstring>& wstr, SOCKET s);
	void veziStats(SOCKET s);

public:
	void startConnection(int port);
	static Connection* getInstance();
	static void destroyInstance();
};
