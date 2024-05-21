#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")

class Client
{
public:
    static Client& getInstance();

    bool connectToServer(const char* ip, int port);
    bool sendData(const char* data);
    bool receiveData(char* buffer, int buffSize);

private:
    SOCKET m_clientSocket;
    static Client* m_instance;

    Client();
    Client(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator=(const Client&) = delete;

    ~Client();
};

#endif // CLIENT_H
