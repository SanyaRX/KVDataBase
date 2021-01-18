#include "pch.h"
#include "KVDBApi.h"

#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void kvdb::KVDBApi::checkResponse(const SOCKET& sock) {

	int serverResponse = -1;
	recv(sock, (char*)&serverResponse, sizeof(serverResponse), 0);

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


void kvdb::KVDBApi::createTable(std::string tableName, const std::vector<std::string>& keys) {

	SOCKET sock;
	
	connectSocket(ipAddress, port, sock);

	sendInt(CREATE_TABLE_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);

	checkResponse(sock);
	
	closesocket(sock);
	WSACleanup();

}


void kvdb::KVDBApi::deleteTable(std::string tableName) {

	SOCKET sock;
	connectSocket(ipAddress, port, sock);

	sendInt(DELETE_TABLE_METHOD_ID, sock);
	sendString(tableName, sock);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

}


void kvdb::KVDBApi::addValue(std::string tableName, const std::vector<std::string>& keys, const std::string& value) {

	SOCKET sock;
	connectSocket(ipAddress, port, sock);

	sendInt(ADD_VALUE_METHOD_ID, sock);

	sendString(tableName, sock);
	sendVector(keys, sock);
	sendString(value, sock);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

}


void kvdb::KVDBApi::deleteAllValuesByKey(std::string tableName, const std::vector<std::string>& keys) {

	SOCKET sock;
	connectSocket(ipAddress, port, sock);

	sendInt(DELETE_ALL_VALUES_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();
}


void kvdb::KVDBApi::deleteValueByKey(std::string tableName, const std::vector<std::string> & keys) {

	SOCKET sock;
	connectSocket(ipAddress, port, sock);

	sendInt(DELETE_VALUE_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();
}


std::vector<std::string> kvdb::KVDBApi::getByKey(std::string tableName, const std::vector<std::string>& keys) {

	SOCKET sock;
	std::vector<std::string> output;

	connectSocket(ipAddress, port, sock);

	sendInt(GET_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);
	
	checkResponse(sock);

	receiveVector(sock, output);
	
	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return output;
}


void kvdb::KVDBApi::updateByKey(std::string tableName, const std::vector<std::string> & keys, const std::string& value) {

	SOCKET sock;
	connectSocket(ipAddress, port, sock);

	sendInt(UPDATE_BY_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(keys, sock);
	sendString(value, sock);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();
}


std::string kvdb::KVDBApi::getFirstValueSorted(std::string tableName, const std::string &key, bool isSorted) {

	SOCKET sock;
	std::string output;
	
	connectSocket(ipAddress, port, sock);

	sendInt(GET_SORTED_VALUE_METHOD_ID, sock);
	sendString(tableName, sock);
	sendString(key, sock);
	sendInt(isSorted, sock);

	checkResponse(sock);

	receiveString(sock, output);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return output;
}


std::string kvdb::KVDBApi::getFirstValue(std::string tableName, const std::string &key) {

	return getFirstValueSorted(tableName, key, false);
}


std::string kvdb::KVDBApi::getLastValue(std::string tableName, const std::string & key) {

	return getFirstValueSorted(tableName, key, true);
}


std::vector<std::string> kvdb::KVDBApi::getFirstKey(std::string tableName) {
	SOCKET sock;
	std::vector<std::string> key;
	
	connectSocket(ipAddress, port, sock);

	sendInt(GET_FIRST_KEY_METHOD_ID, sock);
	sendString(tableName, sock);

	checkResponse(sock);

	receiveVector(sock, key);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return key;
}


std::vector<std::string> kvdb::KVDBApi::getLastKey(std::string tableName) {
	SOCKET sock;
	std::vector<std::string> keys;

	connectSocket(ipAddress, port, sock);

	sendInt(GET_LAST_KEY_METHOD_ID, sock);
	sendString(tableName, sock);

	checkResponse(sock);

	receiveVector(sock, keys);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return keys;
}


std::vector<std::string> kvdb::KVDBApi::getNextKey(std::string tableName, const std::vector<std::string> & currentKeys) {
	SOCKET sock;
	std::vector<std::string> outputKey;

	connectSocket(ipAddress, port, sock);

	sendInt(GET_NEXT_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(currentKeys, sock);

	checkResponse(sock);

	receiveVector(sock, outputKey);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return outputKey;
}


std::vector<std::string> kvdb::KVDBApi::getPrevKey(std::string tableName, const std::vector<std::string> & currentKeys) {
	SOCKET sock;
	std::vector<std::string> outputKey;

	connectSocket(ipAddress, port, sock);

	sendInt(GET_PREV_KEY_METHOD_ID, sock);
	sendString(tableName, sock);
	sendVector(currentKeys, sock);

	checkResponse(sock);

	receiveVector(sock, outputKey);

	checkResponse(sock);

	closesocket(sock);
	WSACleanup();

	return outputKey;
}