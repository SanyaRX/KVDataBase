#pragma once

#ifdef KVDBAPI_EXPORTS
#define KVDB_API __declspec(dllexport)
#else
#define KVDB_API __declspec(dllimport)
#endif

#include "KVDBUtils.h"

#include <string>
#include <vector>

namespace kvdb {

	extern "C" KVDB_API int openDataBase(std::string ipAddress, int port);

	extern "C" KVDB_API int createTable(std::string tableName, const std::vector<std::string> &keys);
	extern "C" KVDB_API int deleteTable(std::string tableName);

	extern "C" KVDB_API int addValue(std::string tableName, const std::vector<std::string>& keys, std::string value);
	extern "C" KVDB_API int deleteAllValuesByKey(std::string tableName, const std::vector<std::string> &keys);
	extern "C" KVDB_API int deleteValueByKey(std::string tableName, const std::vector<std::string> &keys);

	extern "C" KVDB_API int getByKey(std::string tableName, const std::vector<std::string> &keys, std::vector<std::string> &output);
	extern "C" KVDB_API int updateByKey(std::string tableName, const std::vector<std::string> &keys, std::string value);

	extern "C" KVDB_API int getFirstValue(std::string tableName, const std::string & key, std::string & output);
	extern "C" KVDB_API int getLastValue(std::string tableName, const std::string & key, std::string & output);
}