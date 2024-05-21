
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "Connection.h"

Connection* Connection::instance = nullptr;

Connection* Connection::getInstance()
{
	if (instance == nullptr)
		instance = new Connection;
	return instance;
}

void Connection::destroyInstance()
{
	if (instance)
	{
		delete instance;
	}
}

void Connection::startConnection(int port)
{
	if (!listenForAdmin()) return;
	//if (!listenExitAdmin()) return;
	if (!createSocket(port) || !bindSocket(port) || !listenForConnections())
	{
		return;
	}
}

void Connection::loginRegister(std::vector<std::wstring>& wstr, SOCKET s)
{
	Homeworks::setFlag(0);
	if (DB::getInstance()->connect())
	{
		std::cout << "Conectat la baza de date!\n";
		Logger::getInstance()->log("Conectat la baza de date!\n");
	}
	else
	{
		std::cout << "Nu se poate realiza conexiunea cu baza de date!\n";
		Logger::getInstance()->log("Nu se poate realiza conexiunea cu baza de date!\n");
		return;
	}

	IAction* actiune;
	if (Utilitati::selectAction(wstr[0]))
	{
		actiune = FactoryActions::registerAction(wstr);
	}
	else actiune = FactoryActions::loginAction(wstr);

	actiune->manageAction();
	actiune->sendRaspuns(s);
}

void Connection::getHomeworks(std::vector<std::wstring>& wstr, SOCKET s)
{
	Homeworks* homeworks = nullptr;
	homeworks = DB::getInstance()->getHomeworks(wstr[1]);
	if (!homeworks->getTeme().empty())
		Utilitati::sendTeme(homeworks, s);
	else
	{
		send(s, "0", 2, 0);	//nu exista teme
	}
}

void Connection::trimitereTema(std::vector<std::wstring>& wstr, SOCKET s)
{
	std::string filename = "";
	//filename += "C:\\User\\anton\\Desktop\\proiect POO\\MyServer\\server\\rezolvariTeme\\";
	filename = DB::getInstance()->insertRezolvareTema(wstr[1], wstr[2], wstr[3]);

	if (filename != "")
	{
		send(s, "OK", 3, 0);
		std::string s(wstr[4].begin(), wstr[4].end());
		Utilitati::coutFile(filename, s);
	}
	else
	{
		send(s, "Eroare la trimiterea temei!!!", strlen("Eroare la trimiterea temei!!!"), 0);
	}
}

void Connection::veziTemplate(std::vector<std::wstring>& wstr, SOCKET s)
{
	std::string filename = "";
	filename = DB::getInstance()->veziTemplate(wstr[1], wstr[2]);

	if (filename != "")
	{
		//send(clientSocket, "OK", 3, 0);
		std::string buf = Utilitati::readFile(filename);
		if (buf != "")
			send(s, buf.c_str(), buf.size(), 0);
		else
			send(s, "There is no Template!!!", 30, 0);
	}
	else
	{
		// nu exista template pt tema respectiva
		send(s, "There is no Template!!!", 30, 0);
	}
}

void Connection::profesorIncarcaTema(std::vector<std::wstring>& wstr, SOCKET s)
{
	if (DB::getInstance()->insertHomework(wstr[1], wstr[2], wstr[3]))
	{
		send(s, "OK", 3, 0);
		//send(clientSocket, "The homework has been succesfully uploaded!!!",strlen("The homework has been succesfully uploaded!!!" + 1), 0);
		Homeworks::setFlag(0);
	}
	else
	{
		send(s, "Eroare la incarcarea temei!!!", 30, 0);
	}
}

void Connection::incarcareTemplateVectorRezultate(std::vector<std::wstring>& wstr, SOCKET s)
{
	std::string filename1 = "";
	std::string filename2 = "";

	std::pair<std::string, std::string> pereche = DB::getInstance()->insertTemplate(wstr[1], wstr[2]);

	filename1 = pereche.first;
	filename2 = pereche.second;


	if (filename1 != "")
	{
		send(s, "OK", strlen("OK") + 1, 0);
		std::string s(wstr[3].begin(), wstr[3].end());
		Utilitati::coutFile(filename1, s);
	}
	else
	{
		send(s, "OK", strlen("OK" + 1), 0);
	}

	if (filename2 != "")
	{
		std::string s(wstr[4].begin(), wstr[4].end());
		Utilitati::coutFile(filename2, s);
	}
	else
	{
		send(s, "OK", strlen("OK" + 1), 0);
	}
}

void Connection::veziVectorRezultate(std::vector<std::wstring>& wstr, SOCKET s)
{
	std::string filename = "";
	filename = DB::getInstance()->veziVectorRezultate(wstr[1], wstr[2]);

	if (filename != "")
	{
		std::string buf = Utilitati::readFile(filename);
		if (buf != "")
			send(s, buf.c_str(), sizeof(buf), 0);
		else
			send(s, "There is no Vector!!!", 30, 0);
	}
	else
	{
		send(s, "There is no Vector!!!", 30, 0);
	}
}

void Connection::veziRezolvariTema(std::vector<std::wstring>& wstr, SOCKET s)
{
	std::vector<std::pair<std::string, std::string>> vector = DB::getInstance()->getRezolvariTema(wstr[1], wstr[2]);
	if (!vector.empty())
		Utilitati::sendRezolvariTema(vector, s);
	else
	{
		send(s, "0", 2, 0);	//nu exista rezolvari  pt tema respectiva
	}
}

void Connection::procentCheckOutputProf(std::vector<std::wstring>& wstr, SOCKET s)
{
	std::string s1(wstr[1].begin(), wstr[1].end());
	std::string s2(wstr[2].begin(), wstr[2].end());
	std::wstring s3(wstr[3].begin(), wstr[3].end());
	std::wstring s4(wstr[4].begin(), wstr[4].end());
	std::wstring s5(wstr[5].begin(), wstr[5].end());
	std::wstring s6(wstr[6].begin(), wstr[6].end());
	float procent = static_cast<float>(Utilitati::GetSimilitudine(s1, s2));
	DB::getInstance()->insertProcentaj(s3, s4, s5, s6, procent);
	std::string buff = "Procentul de similitudine: " + std::to_string(procent) + "%";
	send(s, buff.c_str(), buff.size(), 0);
}

void Connection::makeRedCommon(std::vector<std::wstring>& wstr, SOCKET s)
{
	std::string s1(wstr[1].begin(), wstr[1].end());
	Utilitati::insertSpaceBeforeEOL(s1);
	std::string s2(wstr[2].begin(), wstr[2].end());
	Utilitati::GetSimilitudine(s1, s2);
	std::string comun100 = Utilitati::readFile(this->fileName100);
	std::string comunPartial = Utilitati::readFile(this->fileName);
	std::cout << "Message sent : " << comun100;
	send(s, comun100.c_str(), comun100.size(), 0);
}

void Connection::veziStats(SOCKET s)
{
	std::vector<std::pair<float, std::pair<std::string, std::string>>> vector = DB::getInstance()->getStatistici();
	if (!vector.empty())
		Utilitati::sendStatistici(vector, s);
	else
	{
		send(s, "0", 2, 0);	//nu exista statistici
	}
}

bool Connection::createSocket(int port) 
{
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) 
	{
		std::cout << "EROARE FATALA -> NU S-A PUTUT CREA SOCKETUL\n";
		Logger::getInstance()->log("EROARE FATALA -> NU S-A PUTUT CREA SOCKETUL\n");
		return false;
	}

	return true;
}

bool Connection::bindSocket(int port)
{
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port); 
	serverAddr.sin_addr.s_addr = INADDR_ANY; 

	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
	{
		std::cout << "EROARE FATALA -> BIND FAILED\n";
		Logger::getInstance()->log("EROARE FATALA -> BIND FAILED\n");
		return false;
	}

	std::cout << "LISTENING...\n";
	Logger::getInstance()->log("LISTENING...\n");
	return true;
}

bool Connection::listenForConnections() 
{
	
	if (listen(serverSocket, SOMAXCONN) == -1) 
	{
		std::cout << "EROARE FATALA -> LISTEN FAILED\n";
		Logger::getInstance()->log("EROARE FATALA -> LISTEN FAILED\n");
		return false;
	}
	std::cout << "WAITING FOR CONNECTIONS...\n";
	Logger::getInstance()->log("WAITING FOR CONNECTIONS...\n");
	
	while (true) 
	{
		int clientSocket = accept(serverSocket, nullptr, nullptr);
		
		if (clientSocket == -1) 
		{
			std::cout << "EROARE -> NU S-A PUTUT REALIZA CONEXIUNEA\n";
			Logger::getInstance()->log("EROARE -> NU S-A PUTUT REALIZA CONEXIUNEA\n");
			continue;
		}

		std::cout << "CONEXIUNE ACCEPTATA PENTRU " << clientSocket << "!!!\n";

		Logger::getInstance()->log("CONEXIUNE ACCEPTATA PENTRU " + std::to_string(clientSocket) + "!!!\n");
		
		std::thread clientThread(&Connection::handleClient, this, clientSocket);
		clientThread.detach(); 

	}
	return true;
}

bool Connection::listenForAdmin()
{
	
	std::thread listenerThread(&Utilitati::ascultaCombinatiiTaste);
	listenerThread.detach();
	return 1;
}

//bool Connection::listenExitAdmin()
//{
//	std::thread listenerThread(&Utilitati::iesireMeniu);
//	listenerThread.detach();
//	return 1;
//}

void Connection::handleClient(int clientSocket) 
{
	char buffer[1024];

	while (true)
	{
		std::cout << "---Asteapta interactiune de la client...---\n";
		Logger::getInstance()->log("---Asteapta interactiune de la client...---\n");
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived == SOCKET_ERROR) 
		{
			std::cout << "EROARE -> RECEIVED FAILED\n";
			Logger::getInstance()->log("EROARE -> RECEIVED FAILED\n");
			break;
		}
		else if (bytesReceived == 0) 
		{
			std::cout << "CLIENT DECONECTAT...\n";
			Logger::getInstance()->log("CLIENT DECONECTAT...\n");
			break;
		}
		else
		{
			buffer[bytesReceived] = '\0'; 

			std::vector<std::wstring> brokenBuffer = Utilitati::breakBuffer(buffer, '$');

			std::cout << "Received data from client: " << buffer << std::endl;
			Logger::getInstance()->log("Received data from client: " + (std::string)buffer + "\n");

			if (brokenBuffer[0] == L"0" || brokenBuffer[0] == L"1")
			{
				this->loginRegister(brokenBuffer, clientSocket);
			}

			else if (brokenBuffer[0] == L"3" || brokenBuffer[0] == L"4" || brokenBuffer[0] == L"5")
			{
				std::string s(brokenBuffer[1].begin(), brokenBuffer[1].end()); //am transformat wstring in string
				Compiler::compile(s, std::stoi(brokenBuffer[0]),clientSocket);
			}

			//studentul/profesorul apasa pe Homeworks
			else if (brokenBuffer[0] == L"H") 
			{
				this->getHomeworks(brokenBuffer, clientSocket);
			}

			// studentul apasa pe trimitere_tema
			else if (brokenBuffer[0] == L"S")  
			{
				this->trimitereTema(brokenBuffer, clientSocket);
			}

			//studentul/profesorul apasa pe vezi_template
			else if (brokenBuffer[0] == L"T")	
			{
				this->veziTemplate(brokenBuffer, clientSocket);
			}

			//profesorul apasa pe incarcare tema
			else if (brokenBuffer[0] == L"A")	
			{
				this->profesorIncarcaTema(brokenBuffer, clientSocket);
			}

			//incarcare template + vector rezultate
			else if (brokenBuffer[0] == L"TT")
			{
				this->incarcareTemplateVectorRezultate(brokenBuffer, clientSocket);
			}

			// vezi vector rezultate
			else if (brokenBuffer[0] == L"V")
			{
				this->veziVectorRezultate(brokenBuffer, clientSocket);
			}
			
			//vector rezolvari pt o tema(nume student + rezolvare)
			else if (brokenBuffer[0] == L"R")
			{
				this->veziRezolvariTema(brokenBuffer, clientSocket);
			}

			//Procent similitudine
			else if (brokenBuffer[0] == L"C")
			{
				this->procentCheckOutputProf(brokenBuffer, clientSocket);
			}

			else if (brokenBuffer[0] == L"CS")
			{
				this->makeRedCommon(brokenBuffer, clientSocket);
			}

			//vezi statistici
			else if (brokenBuffer[0] == L"stat")
			{
				this->veziStats(clientSocket);
			}
			
		}
	}

	closesocket(clientSocket);
}

