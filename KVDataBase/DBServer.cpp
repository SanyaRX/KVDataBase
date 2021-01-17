#include "DBServer.h"


DBServer::DBServer(int portNumber) {

	this->port = portNumber;
}


int DBServer::handleRequest(const SOCKET &clientSocket) {
    int methodID = -1;
    recv(clientSocket, (char*)&methodID, sizeof(methodID), 0);

    std::cout << "Method id: " << methodID << " ";

    std::string tableName;
    std::vector<std::string> keys;
    std::string value;

    switch (methodID) {
    case CREATE_TABLE_METHOD_ID: {
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

        sendInt(KVDB_OK, clientSocket);
        sendVector(data, clientSocket);
        break;
    }
    case UPDATE_BY_KEY_METHOD_ID: {
        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);
        receiveString(clientSocket, value);

        this->database.updateByKeys(tableName, keys, value);
        break;
    }
    case GET_SORTED_VALUE_METHOD_ID: {
        int isSorted = 0;
        std::string key;

        receiveString(clientSocket, tableName);
        receiveString(clientSocket, key);
        recv(clientSocket, (char*)&isSorted, sizeof(isSorted), 0);

        auto response = this->database.getSortedByKey(tableName, key, isSorted);

        sendInt(KVDB_OK, clientSocket);
        sendString(response, clientSocket);
        break;
    }
    case GET_FIRST_KEY_METHOD_ID: {
        receiveString(clientSocket, tableName);

        auto response = this->database.getFirstKey(tableName);
     
        sendInt(KVDB_OK, clientSocket);
        sendVector(response, clientSocket);
        break;
    }
    case GET_LAST_KEY_METHOD_ID: {

        receiveString(clientSocket, tableName);

        auto response = this->database.getLastKey(tableName);

        sendInt(KVDB_OK, clientSocket);
        sendVector(response, clientSocket);
        break;
    }
    case GET_NEXT_KEY_METHOD_ID: {

        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);

        auto response = this->database.getNext(tableName, keys);

        sendInt(KVDB_OK, clientSocket);
        sendVector(response, clientSocket);

        break;
    }
    case GET_PREV_KEY_METHOD_ID: {

        receiveString(clientSocket, tableName);
        receiveVector(clientSocket, keys);

        auto response = this->database.getPrev(tableName, keys);

        sendInt(KVDB_OK, clientSocket);
        sendVector(response, clientSocket);

        break;
    }
    default: std::cout << "No method with id " << methodID << std::endl; break;
    }

    sendInt(KVDB_OK, clientSocket);
    std::cout << "It's ok\n";
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

    
    SOCKET clientSocket;
    
    while (listenSocket) {

        clientSocket = accept(listen_socket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "accept failed: " << WSAGetLastError() << "\n"; 
            closesocket(listen_socket);
            WSACleanup();
        }
        else {
            try {
                handleRequest(clientSocket);
            }
            catch (InvalidKeySizeException ikse) {
                std::cout << "InvalidKeySizeException" << std::endl;
                sendInt(KVDB_INVALID_KEY_SIZE, clientSocket);
            }
            catch (ThereIsNoSuchKeyException nske) {
                std::cout << "ThereIsNoSuchKeyException" << std::endl;
                sendInt(KVDB_NO_SUCH_KEY, clientSocket);
            }
            catch (ThereIsNoSuchTableException nste) {
                std::cout << "ThereIsNoSuchTableException" << std::endl;
                sendInt(KVDB_TABLE_DOESNT_EXISTS, clientSocket);
            }
            catch (TableAlreadyExistsException taee) {
                std::cout << "TableAlreadyExistsException" << std::endl;
                sendInt(KVDB_TABLE_ALREADY_EXISTS, clientSocket);
            }
            catch (...) {
                std::cout << "other exceptions" << std::endl;
                sendInt(KVDB_UNDEFINED_ERROR, clientSocket);
            }
        }

    }

    return 0;
}


int DBServer::closeServer() {
   
    freeaddrinfo(addr);
    WSACleanup();

    return 0;
}