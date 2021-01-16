#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>

#include "KVDBUtils.h"
#include "KVDataBase.h"


#pragma comment(lib, "Ws2_32.lib")

using std::cerr;

class DBServer
{
	Database database;

	int port = 8080;
	
	WSADATA wsaData;
	struct addrinfo* addr = NULL;

	std::vector<std::thread*> threads;
	bool listenSocket = true;

	int handleRequest(const SOCKET& clientSocket);
public:

	DBServer() {};
	DBServer(int portNumber);

	int startServer();
	int closeServer();

};

