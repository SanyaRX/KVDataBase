#include "DBServer.h"

const int max_client_buffer_size = 1024;

DBServer::DBServer(int portNumber) {

	this->port = portNumber;
}


int DBServer::receiveString(const SOCKET &clientSocket, std::string &output) {
    
    char buf[max_client_buffer_size];

    int result = recv(clientSocket, buf, max_client_buffer_size, 0);

    if (result == SOCKET_ERROR) {
        cerr << "recv failed: " << result << "\n";

        return 1;
    }
    else if (result == 0) {
        cerr << "connection closed...\n";
        return 2;
    }
    else if (result > 0) {
        buf[result] = '\0';

        output = std::string(buf);
    }


    return 0;
}


int DBServer::receiveVector(const SOCKET& clientSocket, std::vector<std::string>& output) {

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

int DBServer::sendInt(int value, const SOCKET& clientSocket) {

    int sendResult = send(clientSocket, (char*)&value, sizeof(value), 0);
    return sendResult;
}

int DBServer::sendString(std::string message, const SOCKET& clientSocket) {

    int sendResult = send(clientSocket, message.c_str(), message.size() + 1, 0);
    return sendResult;
}


int DBServer::sendVector(const std::vector<std::string>& vector, const SOCKET& clientSocket) {

    int sendResult;
    int recvResult = 0;

    recv(clientSocket, (char*)&recvResult, sizeof(int), 0);
    sendResult = sendInt(vector.size(), clientSocket);

    if (sendResult <= 0) {
        return -1;
    }


    for (std::string message : vector) {

        recv(clientSocket, (char*)&recvResult, sizeof(int), 0);
        sendString(message, clientSocket);

        if (sendResult <= 0) {
            return -1;
        }
    }


    return 0;
}


int DBServer::handleRequest(const SOCKET &clientSocket) {
    
    int methodID = -1;
    recv(clientSocket, (char*)&methodID, sizeof(methodID), 0);

    std::cout << "MethodID = " << methodID << std::endl;
    std::string tableName;
    std::vector<std::string> keys;
    std::string value = "[[[[[";
    switch (methodID) {

    case CREATE_TABLE_METHOD_ID: {
        // create table
        DBServer::receiveString(clientSocket, tableName);
        DBServer::receiveVector(clientSocket, keys);

        this->database.createTableWithKeys(tableName, keys);
        break;
    }
    case DELETE_TABLE_METHOD_ID: {
        DBServer::receiveString(clientSocket, tableName);

        this->database.deleteTable(tableName);
        break;
    }
    case ADD_VALUE_METHOD_ID: {
        DBServer::receiveString(clientSocket, tableName);
        DBServer::receiveVector(clientSocket, keys);
        DBServer::receiveString(clientSocket, value);
        std::cout << value << std::endl;
        this->database.addValueByKeys(tableName, keys, value);
        break;
    }
    case DELETE_ALL_VALUES_BY_KEY_METHOD_ID: {
        DBServer::receiveString(clientSocket, tableName);
        DBServer::receiveVector(clientSocket, keys);

        this->database.deleteAllValuesByKeys(tableName, keys);
        break;
    }
    case GET_BY_KEY_METHOD_ID: {
        DBServer::receiveString(clientSocket, tableName);
        DBServer::receiveVector(clientSocket, keys);

        auto data = this->database.getByKeys(tableName, keys);
        sendVector(data, clientSocket);
        break;
    }
    case UPDATE_BY_KEY_METHOD_ID: {

        DBServer::receiveString(clientSocket, tableName);
        DBServer::receiveVector(clientSocket, keys);
        DBServer::receiveString(clientSocket, value);
        std::cout << value << std::endl;
        this->database.updateByKeys(tableName, keys, value);
        break;
    }
    case GET_SORTED_VALUE_METHOD_ID: {

        int isSorted = 0;
        std::string key;


        DBServer::receiveString(clientSocket, tableName);
        DBServer::receiveString(clientSocket, key);
        recv(clientSocket, (char*)&isSorted, sizeof(isSorted), 0);

        this->database.getSortedByKey(tableName, key, isSorted);
        break;
    }
    default: std::cout << "No method with id " << methodID << std::endl; break;
    }

    closesocket(clientSocket);
    return 0;
}


int DBServer::startServer() {
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << "\n";
        return result;
    }

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; 
    hints.ai_flags = AI_PASSIVE;


    result = getaddrinfo("127.0.0.1", std::to_string(port).c_str(), &hints, &addr);

    if (result != 0) {
        cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup();
        return 1;
    }

    int listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    
    if (listen_socket == INVALID_SOCKET) {
        cerr << "Error at socket: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        WSACleanup();
        return 1;
    }

    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    if (result == SOCKET_ERROR) {
        cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    
    SOCKET client_socket;
    
    while (listenSocket) {

        client_socket = accept(listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            cerr << "accept failed: " << WSAGetLastError() << "\n"; 
            closesocket(listen_socket);
            WSACleanup();
           // return 1;
        }
        else {
            std::cout << "New client:\n";
            handleRequest(client_socket);
            /*std::thread *newClient = new std::thread(DBServer::handleRequest, client_socket);
            newClient->join();
            
            threads.push_back(newClient);*/
        }

        
        //Sleep(50);
    }

   
   // client_socket = accept(listen_socket, NULL, NULL);
    //handleRequest(client_socket); // multiclient app

    return 0;
}

int DBServer::closeServer() {
   
    freeaddrinfo(addr);
    WSACleanup();

    return 0;
}