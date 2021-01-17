#include "pch.h"
#include "KVDBApi.h"

#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

static std::string ipAddress_;
static int port_;


void checkResponse(const SOCKET& sock) {

	int serverResponse = -1;
	recv(sock, (char*)&serverResponse, sizeof(serverResponse), 0);
	std::cout << "Server response: " << serverResponse << std::endl;

	switch (serverResponse) {
	case KVDB_OK: break;
	case KVDB_TABLE_DOESNT_EXISTS: 
		closesocket(sock);
		WSACleanup();
		throw kvdb_utils::TableDoesNotExistsException(); 
		break;
	case KVDB_TABLE_ALREADY_EXISTS:
		closesocket(sock);
		WSACleanup();
		throw kvdb_utils::TableAlreadyExistsException();
		break;
	case KVDB_INVALID_KEY_SIZE: 
		closesocket(sock);
		WSACleanup();
		throw kvdb_utils::InvalidKeySizeException();
		break;
	case KVDB_NO_SUCH_KEY: 
		closesocket(sock);
		WSACleanup();
		throw kvdb_utils::NoSuchKeyException();
		break;
	case KVDB_UNDEFINED_ERROR:
		closesocket(sock);
		WSACleanup();
		throw kvdb_utils::UndefinedErrorException();
		break;
	default: break;
	}

}


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

	checkResponse(sock);
	
	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::deleteTable(std::string tableName) {

	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(DELETE_TABLE_METHOD_ID, sock);
	sendString(tableName, sock);

	checkResponse(sock);

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

	checkResponse(sock);

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

	checkResponse(sock);

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

	checkResponse(sock);

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
	
	checkResponse(sock);

	receiveVector(sock, output);
	
	checkResponse(sock);

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

	checkResponse(sock);

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

	checkResponse(sock);

	receiveString(sock, output);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::getFirstValue(std::string tableName, const std::string &key, std::string& output) {

	getFirstValueSorted(tableName, key, false, output);

	return 0;
}


int kvdb::getLastValue(std::string tableName, const std::string & key, std::string& output) {

	getFirstValueSorted(tableName, key, true, output);

	return 0;
}


int kvdb::getFirstKey(std::string tableName, std::vector<std::string>& keys) {
	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(GET_FIRST_KEY_METHOD_ID, sock);
	sendString(tableName, sock);

	checkResponse(sock);

	receiveVector(sock, keys);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::getLastKey(std::string	tableName, std::vector<std::string> & keys) {
	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(GET_LAST_KEY_METHOD_ID, sock);
	sendString(tableName, sock);

	checkResponse(sock);

	receiveVector(sock, keys);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::getNextKey(std::string tableName, const std::vector<std::string> & currentKeys, std::vector<std::string> & outputKeys) {
	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(GET_NEXT_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(currentKeys, sock);

	checkResponse(sock);

	receiveVector(sock, outputKeys);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::getPrevKey(std::string tableName, const std::vector<std::string> & currentKeys, std::vector<std::string> & outputKeys) {
	SOCKET sock;
	connectSocket(ipAddress_, port_, sock);

	sendInt(GET_PREV_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(currentKeys, sock);

	checkResponse(sock);

	receiveVector(sock, outputKeys);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}