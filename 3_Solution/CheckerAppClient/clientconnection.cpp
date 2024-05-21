#include "ClientConnection.h"
#include "qdebug.h"
#include "qlogging.h"
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include<string>


ClientConnection* ClientConnection::m_instance = nullptr;

ClientConnection& ClientConnection::getInstance()
{
    if(m_instance == nullptr)
        m_instance = new ClientConnection();

    return *m_instance;
}

bool ClientConnection::connectToServer(const char * ip, int port)
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        Logger::getInstance()->log("WSAStartup failed with error: " + std::to_string(result) + "\n" );
        return 1; // Indicate failure
    }

    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    if (connect(m_clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Connection failed\n";
        Logger::getInstance()->log("Error: Connection failed\n");
        closesocket(m_clientSocket);
        return false;
    }

    std::cout<< "Connected to server!\n";
    Logger::getInstance()->log("Connected to server!\n");
    return true;
}

bool ClientConnection::sendData(const char * buffer)
{
    int bufferLength = strlen(buffer);
    int bytesSent = send(m_clientSocket, buffer, bufferLength, 0);
    if (bytesSent == -1) {
        std::cerr << "Error: Send failed\n";
        Logger::getInstance()->log("Error: Send failed\n");
        return false;
    }
    return true;
}

bool ClientConnection::receiveData() {

    char buffer[1024];
    int bytesRead = recv(m_clientSocket, buffer, sizeof(buffer), 0);
    while(1){
        if (bytesRead == SOCKET_ERROR || bytesRead == 0) {
            std::cerr << "Error: Receive failed\n";
            Logger::getInstance()->log("Error: Receive failed\n");
            return false;
        }else{

            buffer[bytesRead] = '\0';
            std::cout<<buffer;
            qDebug() << "Message received: " << buffer;
            Logger::getInstance()->log("Message received: " + (std::string)buffer + "\n");
             break;
        }
    }

    int i=0;
    if(strstr(buffer, "ok")){
        while(buffer[i]!=NULL){
            if(buffer[i]=='#'){
                this->typeAuthentication=buffer[i+1];
                break;
            }
            i++;
        }
    }

    if(strstr(buffer, "ok"))
        return true;
    else
        return false;
}


std::string ClientConnection::receiveDataCompilate() {

    char buffer[10240];
    int bytesRead = recv(m_clientSocket, buffer, sizeof(buffer), 0);
    while(1){
        if (bytesRead == SOCKET_ERROR || bytesRead == 0) {
            std::cerr << "Error: Receive failed\n";
            Logger::getInstance()->log("Error: Receive failed\n");
            exit(0);
        }else{

            buffer[bytesRead] = '\0';
            std::cout<<buffer;
            qDebug() << "Message received: " << buffer;
            Logger::getInstance()->log("Message received: " + (std::string)buffer + "\n");
            break;
        }
    }

    std::string s(buffer);

    return s;
}

std::string ClientConnection::receiveDataHomeworks()
{
    std::string buffer;
    char recvBuffer[10000];

    int bytesRead = recv(m_clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    while(1){
        if (bytesRead == SOCKET_ERROR || bytesRead == 0) {
            std::cerr << "Error: Receive failed\n";
            Logger::getInstance()->log("Error: Receive failed\n");
            exit(0);
        }else{

            recvBuffer[bytesRead] = '\0';
            std::cout<<recvBuffer;
            qDebug() << "Message received: " << recvBuffer;
            Logger::getInstance()->log("Message received: " + (std::string)buffer + "\n");
            buffer.assign(recvBuffer, bytesRead);
            break;
            }
    }

    return buffer;
}

ClientConnection:: ~ClientConnection()
{
    closesocket(m_clientSocket);
}
