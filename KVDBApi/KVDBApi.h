#pragma once

#include "KVDBUtils.h"
#include "KVDBError.h"

#include <string>
#include <vector>

namespace kvdb {

	class KVDBApi {

		std::string ipAddress = "127.0.0.1";
		int port = 8080;

		std::string getFirstValueSorted(const std::string& tableName, const std::string& key, bool isSorted);
		void checkResponse(const SOCKET& sock);
	public:
		KVDBApi() {};
		KVDBApi(const std::string& ipAddress, int port) : ipAddress(ipAddress), port(port) {};

		void createTable(const std::string& tableName, const std::vector<std::string>& keys);
		void deleteTable(const std::string& tableName);

		void addValue(const std::string& tableName, const std::vector<std::string>& keys, const std::string& value);
		void deleteAllValuesByKey(const std::string& tableName, const std::vector<std::string>& keys);
		void deleteValueByKey(const std::string& tableName, const std::vector<std::string>& keys);

		std::vector<std::string> getByKey(const std::string& tableName, const std::vector<std::string>& keys);
		void updateByKey(const std::string& tableName, const std::vector<std::string>& keys, const std::string& value);

		std::string getFirstValue(const std::string& tableName, const std::string& key);
		std::string getLastValue(const std::string& tableName, const std::string& key);

		std::vector<std::string> getFirstKey(const std::string& tableName);
		std::vector<std::string> getLastKey(const std::string& tableName);

		std::vector<std::string> getNextKey(const std::string& tableName, const std::vector<std::string>& currentKeys);
		std::vector<std::string> getPrevKey(const std::string& tableName, const std::vector<std::string>& currentKeys);
	};
}