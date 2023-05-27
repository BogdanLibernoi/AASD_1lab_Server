#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "Patient.h"
#include "Queue.h"
#include "PatientsHandler.h"

#pragma comment(lib, "ws2_32.lib")

SOCKET ConnectSocket;

void StartWorking(PatientsHandler action, SOCKET sock)
{
    char msg[512] = " ";

    while(true) {
        int check = recv(sock, msg, sizeof(msg), NULL);
        Sleep(1000);
        std::cout << msg;
        //std::cout << check;
        if (check != -1)
        {
            std::string temp = action.GetPatient();
            //while (temp != "queue is empty")
            //{
                strcpy_s(msg, temp.c_str());
                send(sock, msg, sizeof(msg), NULL);
           // }
        }

        else if (check == -1)
        {
            std::cout << "Can't accept message. Error #" << WSAGetLastError();
            break;
        }
    }
}

int main()
{
    PatientsHandler handler;

#pragma region Socket Connection

    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cout << "\nWSAStartup failed." << std::endl;
        std::cout << WSAGetLastError();
        return 1;
    }
    else
        std::cout << "\nWSAStartup successful" << std::endl;

    ConnectSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (ConnectSocket == INVALID_SOCKET)
    {
        std::cout << "Socket initialization error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    in_addr ip_to_num;
    result = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
    if (result <= 0)
    {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return 1;
    }

    sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));

    // Инициализация  IP-адреса, порта, семейства адресов
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons('1119');

    // Привязка IP, port(a) и семейства адресов к сокету
    result = bind(ConnectSocket, (sockaddr*)&servInfo, sizeof(servInfo));
    if (result != 0)
    {
        std::cout << "Error binding to server info. Error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Binding socket to Server info is OK" << std::endl;

    //Включение прослушивания привязанного порта для идентификации подключений
    result = listen(ConnectSocket, SOMAXCONN);

    if (result != 0)
    {
        std::cout << "Can't start to listen to, Error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Listening... " << std::endl << std::endl;

    sockaddr_in clientInfo;
    int size_clientInfo = sizeof(clientInfo);

    ZeroMemory(&clientInfo, size_clientInfo);

    SOCKET clientConnection;


    clientConnection = accept(ConnectSocket, (sockaddr*)&clientInfo, &size_clientInfo);

    if (clientConnection == INVALID_SOCKET)
    {
        std::cout << "Client detected, but can't connect to a client. Error: " << WSAGetLastError() << std::endl;
        closesocket(clientConnection);
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Connection to a client successful" << std::endl;
        StartWorking(handler, clientConnection);
    }

#pragma endregion

    system("pause");
}

