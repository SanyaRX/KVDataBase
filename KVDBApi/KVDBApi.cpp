#include "pch.h"
#include "KVDBApi.h"

#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

static std::string ipAddress_;
static int port_;

const int max_client_buffer_size = 1024;


int connectSocket(SOCKET &sock) {
	
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return 1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port_);
	inet_pton(AF_INET, ipAddress_.c_str(), &hint.sin_addr);

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
}


int receiveString(const SOCKET& clientSocket, std::string& output) {

	char buf[max_client_buffer_size];

	int result = recv(clientSocket, buf, max_client_buffer_size, 0);

	if (result == SOCKET_ERROR) {
		return 1;
	}
	else if (result == 0) {
		return 2;
	}
	else if (result > 0) {
		buf[result] = '\0';

		output = std::string(buf);
	}


	return 0;
}



int receiveVector(const SOCKET& clientSocket, std::vector<std::string>& output) {

	int ok = 0;
	send(clientSocket, (char*)&ok, sizeof(int), 0);

	int vectorSize = 0;
	recv(clientSocket, (char*)&vectorSize, sizeof(vectorSize), 0);
	output = std::vector<std::string>();
	std::cout << vectorSize << std::endl;

	int receiveResult = 0;

	std::string message;
	for (int i = 0; i < vectorSize; i++) {

		send(clientSocket, (char*)&ok, sizeof(int), 0);

		receiveResult = receiveString(clientSocket, message);

		output.push_back(message);
	}

	return 0;
}


int sendIntToServer(int value, const SOCKET& socket_) {

	int sendResult = send(socket_, (char*)&value, sizeof(value), 0);
	return sendResult;
}

int sendStringToServer(std::string message, const SOCKET &socket_) {

	int sendResult = send(socket_, message.c_str(), message.size() + 1, 0);

	return sendResult;
}


int sendVectorToServer(const std::vector<std::string> &vector, const SOCKET& socket_) {

	int sendResult;
	int recvResult = 0;

	recv(socket_, (char*)&recvResult, sizeof(int), 0);
	sendResult = sendIntToServer(vector.size(), socket_);

	if (sendResult <= 0) {
		return -1;
	}


	for (std::string message : vector) {
		
		recv(socket_, (char *)&recvResult, sizeof(int), 0);
		sendStringToServer(message, socket_);

		if (sendResult <= 0) {
			return -1;
		}
	}

	return 0;
}



int kvdb::openDataBase(std::string ipAddress, int port) {

	ipAddress_ = ipAddress;
	port_ = port;

	return 0;
}


int kvdb::createTable(std::string tableName, const std::vector<std::string>& keys) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(CREATE_TABLE_METHOD_ID, sock);
	sendStringToServer(tableName, sock);
	sendVectorToServer(keys, sock);

	
	closesocket(sock);
	WSACleanup();

	return 0;
}


int kvdb::deleteTable(std::string tableName) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(DELETE_TABLE_METHOD_ID, sock);
	sendStringToServer(tableName, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::addValue(std::string tableName, const std::vector<std::string>& keys, std::string value) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(ADD_VALUE_METHOD_ID, sock);

	sendStringToServer(tableName, sock);
	sendVectorToServer(keys, sock);
	sendStringToServer(value, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::deleteAllValuesByKey(std::string tableName, const std::vector<std::string>& keys) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(DELETE_ALL_VALUES_BY_KEY_METHOD_ID, sock);
	sendStringToServer(tableName, sock);
	sendVectorToServer(keys, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::deleteValueByKey(std::string tableName, const std::vector<std::string> & keys) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(DELETE_VALUE_BY_KEY_METHOD_ID, sock);
	sendStringToServer(tableName, sock);
	sendVectorToServer(keys, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::getByKey(std::string tableName, const std::vector<std::string> & keys, std::vector<std::string> & output) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(GET_BY_KEY_METHOD_ID, sock);
	sendStringToServer(tableName, sock);
	sendVectorToServer(keys, sock);
	
	receiveVector(sock, output);
	
	closesocket(sock);
	WSACleanup();

	return 0;
}

int kvdb::updateByKey(std::string tableName, const std::vector<std::string> & keys, std::string value) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(UPDATE_BY_KEY_METHOD_ID, sock);
	sendStringToServer(tableName, sock);
	sendVectorToServer(keys, sock);
	sendStringToServer(value, sock);

	closesocket(sock);
	WSACleanup();

	return 0;
}


int getFirstValueSorted(std::string tableName, const std::string &key, bool isSorted, std::string &output) {

	SOCKET sock;
	connectSocket(sock);

	sendIntToServer(GET_SORTED_VALUE_METHOD_ID, sock);
	sendStringToServer(tableName, sock);
	sendStringToServer(key, sock);
	sendIntToServer(isSorted, sock);

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