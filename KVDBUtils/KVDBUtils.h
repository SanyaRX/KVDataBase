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
#define GET_FIRST_KEY_METHOD_ID 9
#define GET_LAST_KEY_METHOD_ID 10
#define GET_NEXT_KEY_METHOD_ID 11
#define GET_PREV_KEY_METHOD_ID 12


int connectSocket(std::string ipAddress, int port, SOCKET& sock);

int receiveString(const SOCKET& sock, std::string& output);

int receiveVector(const SOCKET& sock, std::vector<std::string>& output);

int sendInt(int value, const SOCKET& sock);

int sendString(std::string message, const SOCKET& sock);

int sendVector(const std::vector<std::string>& vector, const SOCKET& sock);
