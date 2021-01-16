#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include "windows.h"

using namespace std;
namespace fs = std::filesystem;

class Database
{
public:
	Database();
	Database(string path);
	bool createTableWithKeys(string table_name, vector<string> keys);
	bool deleteTable(string table_name);
	void addValueByKeys(string table_name, vector<string> keys, string value);
	void deleteAllValuesByKeys(string table_name, vector<string> keys);
	void deleteValueByKeys(string table_name, vector<string> keys);
	string getSortedByKey(string table_name, string key, bool isFirst);
	vector<string> getByKeys(string table_name, vector<string> keys);
	void updateByKeys(string table_name, vector<string> key, string value);
	vector<vector<string>> getAllKeys(string table_name);
private:
	string dots = "..";
	string doubleslash = "\\";
	string dottxt = ".txt";
	string metasymbol = ",";
	char token = ',';

	
	void eraseSubStr(std::string& mainStr, const std::string& toErase);
	bool createDirectory(string path);
	bool createTable(string table_name);
	string concatKeys(vector<string> keys);
	void addValue(string table_name, string key, string value);
	void deleteAllValuesByKey(string table_name, string key);
	void deleteValueByKey(string table_name, string key);
	vector<string> getByKey(string table_name, string key);
	void updateByKey(string table_name, string key, string value);
	vector<string> split(const char* str);
	int find(vector<string> vector, string find);
};