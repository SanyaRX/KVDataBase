#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "KVDataBase.h"

#define CREATE_TABLE_METHOD_ID 1
#define DELETE_TABLE_METHOD_ID 2
#define ADD_VALUE_METHOD_ID 3
#define DELETE_ALL_VALUES_BY_KEY_METHOD_ID 4
#define DELETE_VALUE_BY_KEY_METHOD_ID 5
#define GET_BY_KEY_METHOD_ID 6
#define UPDATE_BY_KEY_METHOD_ID 7
#define GET_SORTED_VALUE_METHOD_ID 8

#pragma comment(lib, "Ws2_32.lib")

using std::cerr;

class DBServer
{

	Database database;

	int port = 8080;
	
	WSADATA wsaData;
	struct addrinfo* addr = NULL; // структура, хранящая информацию
// об IP-адресе  слущающего сокета

	std::vector<std::thread*> threads;
	bool listenSocket = true;

	// place database here

	int handleRequest(const SOCKET& clientSocket);

	static int sendVector(const std::vector<std::string>& vector, const SOCKET& clientSocket);
	static int sendInt(int value, const SOCKET& clientSocket);
	static int sendString(std::string message, const SOCKET& clientSocket);


	static int receiveString(const SOCKET &clientSocket, std::string &output);
	static int receiveVector(const SOCKET& clientSocket, std::vector<std::string>& output);
public:

	DBServer() {};
	DBServer(int portNumber);

	int startServer();
	int closeServer();

};

