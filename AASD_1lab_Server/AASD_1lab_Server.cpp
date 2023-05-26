#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "Patient.h"
#include "Queue.h"
#include "PatientsHandler.h"

#pragma comment(lib, "ws2_32.lib")

SOCKET ConnectSocket;

void StartWorking(PatientsHandler action)
{
    char msg[512] = " ";

    while(true) {
        int check = recv(ConnectSocket, msg, sizeof(msg), NULL);
        Sleep(1000);
        std::cout << msg;
        //std::cout << check;
        if (check != -1)
        {
            strcpy_s(msg, action.GetPatient().c_str());
            send(ConnectSocket, msg, sizeof(msg), NULL);
        }
    }
}

int main()
{
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
    servInfo.sin_port = htons('1110');

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
#pragma endregion

    PatientsHandler handler;

    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartWorking, (LPVOID)(&handler), NULL, NULL);
    
    StartWorking(handler);

    system("pause");
}

