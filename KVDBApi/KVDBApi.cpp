#include "pch.h"
#include "KVDBApi.h"

#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

static std::string ipAddress_;
static int port_;


int kvdb::openDataBase(std::string ipAddress, int port) {

	ipAddress_ = ipAddress;
	port_ = port;

	return 0;
}


int kvdb::createTable(std::string tableName, const std::vector<std::string>& keys) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(CREATE_TABLE_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);

	
	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::deleteTable(std::string tableName) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(DELETE_TABLE_METHOD_ID, sock);
	sendString(tableName, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::addValue(std::string tableName, const std::vector<std::string>& keys, std::string value) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(ADD_VALUE_METHOD_ID, sock);

	sendString(tableName, sock);
	sendVector(keys, sock);
	sendString(value, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::deleteAllValuesByKey(std::string tableName, const std::vector<std::string>& keys) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(DELETE_ALL_VALUES_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::deleteValueByKey(std::string tableName, const std::vector<std::string> & keys) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(DELETE_VALUE_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::getByKey(std::string tableName, const std::vector<std::string> & keys, std::vector<std::string> & output) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(GET_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);
	
	receiveVector(sock, output);
	
	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::updateByKey(std::string tableName, const std::vector<std::string> & keys, std::string value) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(UPDATE_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);
	sendString(value, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}


int getFirstValueSorted(std::string tableName, const std::string &key, bool isSorted, std::string &output) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(GET_SORTED_VALUE_METHOD_ID, sock);
	sendString(tableName, sock);
	sendString(key, sock);
	sendInt(isSorted, sock);

	receiveString(sock, output);

	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::getFirstValue(std::string tableName, const std::string &key, std::string& output) {

	getFirstValueSorted(tableName, key, true, output);

	return 0;
}

int kvdb::getLastValue(std::string tableName, const std::string & key, std::string& output) {

	getFirstValueSorted(tableName, key, false, output);

	return 0;
}