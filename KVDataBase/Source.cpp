#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include "windows.h"

using namespace std;
namespace fs = std::filesystem;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

string dots = "..";
string doubleslash = "\\";
string dottxt = ".txt";

void eraseSubStr(std::string& mainStr, const std::string& toErase) {
	size_t pos = mainStr.find(toErase);
	if (pos != std::string::npos) {
		mainStr.erase(pos, toErase.length());
	}
}

bool createDirectory(string path) {
	return CreateDirectory(wstring(path.begin(), path.end()).c_str(), NULL);
}

bool createTable(string table_name) {
	return createDirectory(dots + doubleslash + table_name);
}

bool deleteTable(string table_name) {
	return fs::remove_all(dots + doubleslash + table_name);
}

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

void deleteAllValuesByKey(string table_name, string key) {
	fs::remove_all(dots + doubleslash + table_name + doubleslash);
}

void deleteValueByKey(string table_name, string key) {
	string path = dots + doubleslash + table_name + doubleslash + key;

	for (auto& p : fs::directory_iterator(path)) {
		string s = p.path().u8string();
		DeleteFile(wstring(s.begin(), s.end()).c_str());
		return;
	}	
}

string getSortedInTable(string table_name, bool isFirst) {
	string path = dots + doubleslash + table_name;
	string first_path = "";

	for(auto& p: fs::directory_iterator(path)) {
		if (!fs::is_empty(p.path().u8string())) {
			first_path = p.path().u8string();
			if (isFirst) {
				break;
			}
		}
	}

	if (first_path != "") {
		for (auto& p : fs::directory_iterator(first_path)) {
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

int main()
{
	for (auto& p : getByKey("aue", "new_key")) {
		cout << p << endl;
	}
	
}

