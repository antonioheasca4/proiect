#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include "Logger.h"

class ClientConnection
{
public:
    static ClientConnection& getInstance();

    bool connectToServer(const char* ip, int port);
    bool sendData(const char *);
    bool receiveData();
    char getTypeAuhhentification() { return typeAuthentication; }
    std::string receiveDataCompilate();
    std::string receiveDataHomeworks();

private:

    int m_clientSocket;
    char typeAuthentication;
    static ClientConnection* m_instance;


    ClientConnection() = default;
    ClientConnection(const ClientConnection&) = delete;
    ClientConnection(ClientConnection&&) = delete;
    ClientConnection& operator=(const ClientConnection&) = delete;

    ~ClientConnection();
};

#endif // CLIENTCONNECTION_H
