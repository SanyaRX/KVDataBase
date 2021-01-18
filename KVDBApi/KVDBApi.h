#pragma once

#include "KVDBUtils.h"
#include "KVDBError.h"

#include <string>
#include <vector>

namespace kvdb {

	class KVDBApi {

		std::string ipAddress = "127.0.0.1";
		int port = 8080;

		std::string getFirstValueSorted(std::string tableName, const std::string& key, bool isSorted);
		void checkResponse(const SOCKET& sock);
	public:
		KVDBApi() {};
		KVDBApi(std::string ipAddress, int port) : ipAddress(ipAddress), port(port) {};

		void createTable(std::string tableName, const std::vector<std::string>& keys);
		void deleteTable(std::string tableName);

		void addValue(std::string tableName, const std::vector<std::string>& keys, const std::string& value);
		void deleteAllValuesByKey(std::string tableName, const std::vector<std::string>& keys);
		void deleteValueByKey(std::string tableName, const std::vector<std::string>& keys);

		std::vector<std::string> getByKey(std::string tableName, const std::vector<std::string>& keys);
		void updateByKey(std::string tableName, const std::vector<std::string>& keys, const std::string& value);

		std::string getFirstValue(std::string tableName, const std::string& key);
		std::string getLastValue(std::string tableName, const std::string& key);

		std::vector<std::string> getFirstKey(std::string tableName);
		std::vector<std::string> getLastKey(std::string	tableName);

		std::vector<std::string> getNextKey(std::string tableName, const std::vector<std::string>& currentKeys);
		std::vector<std::string> getPrevKey(std::string tableName, const std::vector<std::string>& currentKeys);
	};
}