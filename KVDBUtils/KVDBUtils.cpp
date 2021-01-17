// KVDBUtils.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include "KVDBUtils.h"


const int max_client_buffer_size = 1024;


int connectSocket(std::string ipAddress, int port, SOCKET& sock) {

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		return 1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		WSACleanup();
		return 1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		return 1;
	}
}


int sendInt(int value, const SOCKET& sock) {

	int sendResult = send(sock, (char*)&value, sizeof(value), 0);
	return sendResult;
}


int sendString(std::string message, const SOCKET& sock) {

	int recvResult = 0;

	recv(sock, (char*)&recvResult, sizeof(int), 0);
	int sendResult = send(sock, message.c_str(), message.size() + 1, 0);

	return sendResult;
}


int sendVector(const std::vector<std::string>& vector, const SOCKET& sock) {

	int sendResult;
	int recvResult = 0;

	recv(sock, (char*)&recvResult, sizeof(int), 0);
	sendResult = sendInt(vector.size(), sock);

	if (sendResult <= 0) {
		return -1;
	}


	for (std::string message : vector) {

		recv(sock, (char*)&recvResult, sizeof(int), 0);
		sendString(message, sock);

		if (sendResult <= 0) {
			return -1;
		}
	}

	return 0;
}
	


int receiveString(const SOCKET& sock, std::string& output) {

	int ok = 0;
	send(sock, (char*)&ok, sizeof(int), 0);

	char buf[max_client_buffer_size];

	int result = recv(sock, buf, max_client_buffer_size, 0);

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



int receiveVector(const SOCKET& sock, std::vector<std::string>& output) {

	int ok = 0;
	send(sock, (char*)&ok, sizeof(int), 0);

	int vectorSize = 0;
	recv(sock, (char*)&vectorSize, sizeof(vectorSize), 0);
	output = std::vector<std::string>();

	int receiveResult = 0;

	std::string message;
	for (int i = 0; i < vectorSize; i++) {

		send(sock, (char*)&ok, sizeof(int), 0);

		receiveResult = receiveString(sock, message);

		output.push_back(message);
	}

	return 0;
}


