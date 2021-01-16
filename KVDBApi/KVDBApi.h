#pragma once

#ifdef KVDBAPI_EXPORTS
#define KVDB_API __declspec(dllexport)
#else
#define KVDB_API __declspec(dllimport)
#endif

#define CREATE_TABLE_METHOD_ID 1
#define DELETE_TABLE_METHOD_ID 2
#define ADD_VALUE_METHOD_ID 3
#define DELETE_ALL_VALUES_BY_KEY_METHOD_ID 4
#define DELETE_VALUE_BY_KEY_METHOD_ID 5
#define GET_BY_KEY_METHOD_ID 6
#define UPDATE_BY_KEY_METHOD_ID 7
#define GET_SORTED_VALUE_METHOD_ID 8

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