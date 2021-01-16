#pragma once
#include <string>
#include <vector>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define CREATE_TABLE_METHOD_ID 1
#define DELETE_TABLE_METHOD_ID 2
#define ADD_VALUE_METHOD_ID 3
#define DELETE_ALL_VALUES_BY_KEY_METHOD_ID 4
#define DELETE_VALUE_BY_KEY_METHOD_ID 5
#define GET_BY_KEY_METHOD_ID 6
#define UPDATE_BY_KEY_METHOD_ID 7
#define GET_SORTED_VALUE_METHOD_ID 8


int connectSocket(std::string ipAddress, int port, SOCKET& sock);

int receiveString(const SOCKET& clientSocket, std::string& output);

int receiveVector(const SOCKET& clientSocket, std::vector<std::string>& output);

int sendInt(int value, const SOCKET& socket_);

int sendString(std::string message, const SOCKET& socket_);

int sendVector(const std::vector<std::string>& vector, const SOCKET& socket_);
