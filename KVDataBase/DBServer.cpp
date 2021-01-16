#include "DBServer.h"

DBServer::DBServer(int portNumber) {

	this->port = portNumber;
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
        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);

        this->database.createTableWithKeys(tableName, keys);
        break;
    }
    case DELETE_TABLE_METHOD_ID: {
        receiveString(clientSocket, tableName);

        this->database.deleteTable(tableName);
        break;
    }
    case ADD_VALUE_METHOD_ID: {
        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);
        receiveString(clientSocket, value);
        std::cout << value << std::endl;
        this->database.addValueByKeys(tableName, keys, value);
        break;
    }
    case DELETE_ALL_VALUES_BY_KEY_METHOD_ID: {
        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);

        this->database.deleteAllValuesByKeys(tableName, keys);
        break;
    }
    case GET_BY_KEY_METHOD_ID: {
        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);

        auto data = this->database.getByKeys(tableName, keys);
        sendVector(data, clientSocket);
        break;
    }
    case UPDATE_BY_KEY_METHOD_ID: {

        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);
        receiveString(clientSocket, value);
        std::cout << value << std::endl;
        this->database.updateByKeys(tableName, keys, value);
        break;
    }
    case GET_SORTED_VALUE_METHOD_ID: {

        int isSorted = 0;
        std::string key;

        receiveString(clientSocket, tableName);
        receiveString(clientSocket, key);
        recv(clientSocket, (char*)&isSorted, sizeof(isSorted), 0);

        std::string response = this->database.getSortedByKey(tableName, key, isSorted);
        sendString(response, clientSocket);
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