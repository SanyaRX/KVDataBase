#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include "windows.h"

using namespace std;
namespace fs = std::filesystem;

string dots = "..";
string doubleslash = "\\";
string dottxt = ".txt";
string metasymbol = ",";
char token = ',';

//private
void eraseSubStr(std::string& mainStr, const std::string& toErase) {
	size_t pos = mainStr.find(toErase);
	if (pos != std::string::npos) {
		mainStr.erase(pos, toErase.length());
	}
}

//private
bool createDirectory(string path) {
	return CreateDirectory(wstring(path.begin(), path.end()).c_str(), NULL);
}

//private
bool createTable(string table_name) {
	return createDirectory(dots + doubleslash + table_name);
}

//private
string concatKeys(vector<string> keys) {
	string key = "";

	for (auto p : keys) {
		key += p + metasymbol;
	}
	return key;
}

//private
void addValue(string table_name, string key, string value) {
	string path = dots + doubleslash + table_name + doubleslash + key;
	string last = "0";
	createDirectory(path);
	
	for (auto& p : fs::directory_iterator(path))
		last = p.path().u8string();

	eraseSubStr(last, path + doubleslash);
	eraseSubStr(last, dottxt);

	int current = stoi(last) + 1;
	
	ofstream myfile(path + doubleslash + to_string(current) + dottxt);
	if (myfile.is_open()) {
		myfile << value;
		myfile.close();
	}
}

//private
void deleteAllValuesByKey(string table_name, string key) {
	fs::remove_all(dots + doubleslash + table_name + doubleslash);
}

//private
void deleteValueByKey(string table_name, string key) {
	string path = dots + doubleslash + table_name + doubleslash + key;

	for (auto& p : fs::directory_iterator(path)) {
		string s = p.path().u8string();
		DeleteFile(wstring(s.begin(), s.end()).c_str());
		return;
	}	
}

//private
vector<string> getByKey(string table_name, string key) {
	string path = dots + doubleslash + table_name + doubleslash + key;
	vector<string> vector;

	for (auto& p : fs::directory_iterator(path)) {
		ifstream myfile(p.path().u8string());
		string line;
		string all = "";
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				all += line;
			}
			myfile.close();
			vector.push_back(all);
		}
	}
	return vector;
}

//private
void updateByKey(string table_name, string key, string value) {
	string path = dots + doubleslash + table_name + doubleslash + key;
	for (auto& p : fs::directory_iterator(path)) {
		ifstream myfile(p.path().u8string());
		string line;
		string all = "";
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				all += line;
			}
			myfile.close();
			return;
		}
	}
}

//private 
vector<string> split(const char* str) {
	vector<string> result;

	do {
		const char* begin = str;

		while (*str != token && *str)
			str++;

		result.push_back(string(begin, str));
	} while (0 != *str++);

	return result;
}

//private 
int find(vector<string> vector, string find) {
	for (int i = 0; i < vector.size(); i++) {
		if (find == vector[i])
			return i;
	}
	return 0;
}

bool createTableWithKeys(string table_name, vector<string> keys) {
	ofstream myfile(dots + doubleslash + table_name + dottxt);
	if (myfile.is_open()) {		
		for (auto p : keys) {
			myfile << p << endl;
		}
		myfile.close();
		return createTable(table_name);
	}
	return false;
}

bool deleteTable(string table_name) {
	return fs::remove_all(dots + doubleslash + table_name);
}

void addValueByKeys(string table_name, vector<string> keys, string value) {
	addValue(table_name, concatKeys(keys), value);
}

void deleteAllValuesByKeys(string table_name, vector<string> keys) {
	deleteAllValuesByKey(table_name, concatKeys(keys));
}

void deleteValueByKeys(string table_name, vector<string> keys) {
	deleteValueByKey(table_name, concatKeys(keys));
}

string getSortedByKey(string table_name, string key, bool isFirst) {
	vector<string> vector;

	ifstream myfile(dots + doubleslash + table_name + dottxt);
	string line;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			vector.push_back(line);
		}
		myfile.close();	
	}
	int index = find(vector, key);
	string sortedString = "";
	string sortedPath = "";
	string temp;
	
	for (auto& p : fs::directory_iterator(dots + doubleslash + table_name)) {
		vector = split(p.path().u8string().c_str());
		temp = vector[index];
		if (sortedString == "" || ((sortedString < temp) == isFirst)) {
			sortedString = temp;
			sortedPath = p.path().u8string();
		}
	}

	if (sortedPath != "") {
		for (auto& p : fs::directory_iterator(sortedPath)) {
			ifstream myfile(p.path().u8string());
			string line;
			string all = "";
			if (myfile.is_open()) {
				while (getline(myfile, line)) {
					all += line;
				}
				myfile.close();
				return all;
			}
		}
	}
	return "";
}

vector<string> getByKeys(string table_name, vector<string> keys) {
	return getByKey(table_name, concatKeys(keys));
}

void updateByKeys(string table_name, vector<string> key, string value) {
	updateByKey(table_name, concatKeys(key), value);
}

int main() {
	vector<string> vector;
	vector.push_back("a2");
	vector.push_back("b2");
	/*getByKeys("table_name", vector);*/

	for (auto p : getByKeys("table_name", vector)) {
		cout << p << endl;
	}

	
}

