#include "KVDataBase.h"

Database:: Database() {}

void Database::eraseSubStr(std::string& mainStr, const std::string& toErase) {
	size_t pos = mainStr.find(toErase);
	if (pos != std::string::npos) {
		mainStr.erase(pos, toErase.length());
	}
}

//private
bool Database::createDirectory(string path) {
	return CreateDirectory(wstring(path.begin(), path.end()).c_str(), NULL);
}

//private
bool Database::createTable(string table_name) {
	return createDirectory(dots + doubleslash + table_name);
}

//private
string Database::concatKeys(vector<string> keys) {
	string key = "";

	for (auto p : keys) {
		key += p + metasymbol;
	}
	return key;
}

//private
void Database::addValue(string table_name, string key, string value) {
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
void Database::deleteAllValuesByKey(string table_name, string key) {
	fs::remove_all(dots + doubleslash + table_name + doubleslash);
}

//private
void Database::deleteValueByKey(string table_name, string key) {
	string path = dots + doubleslash + table_name + doubleslash + key;

	for (auto& p : fs::directory_iterator(path)) {
		string s = p.path().u8string();
		DeleteFile(wstring(s.begin(), s.end()).c_str());
		return;
	}
}

//private
vector<string> Database::getByKey(string table_name, string key) {
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
void Database::updateByKey(string table_name, string key, string value) {
	string path = dots + doubleslash + table_name + doubleslash + key;
	for (auto& p : fs::directory_iterator(path)) {
		ofstream myfile(p.path().u8string());
		if (myfile.is_open()) {
			myfile << value;
			myfile.close();
			return;
		}
	}
}

//private 
vector<string> Database::split(const char* str) {
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
int Database::find(vector<string> vector, string find) {
	for (int i = 0; i < vector.size(); i++) {
		if (find == vector[i])
			return i;
	}
	return 0;
}

bool Database::createTableWithKeys(string table_name, vector<string> keys) {
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

// delete metafile too
bool Database::deleteTable(string table_name) {
	return fs::remove_all(dots + doubleslash + table_name);
}

void Database::addValueByKeys(string table_name, vector<string> keys, string value) {
	addValue(table_name, concatKeys(keys), value);
}

// do not delete folder as we can't add new value
void Database::deleteAllValuesByKeys(string table_name, vector<string> keys) {
	deleteAllValuesByKey(table_name, concatKeys(keys));
}

void Database::deleteValueByKeys(string table_name, vector<string> keys) {
	deleteValueByKey(table_name, concatKeys(keys));
}

string Database::getSortedByKey(string table_name, string key, bool isFirst) {
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

vector<string> Database::getByKeys(string table_name, vector<string> keys) {
	return getByKey(table_name, concatKeys(keys));
}

void Database::updateByKeys(string table_name, vector<string> key, string value) {
	updateByKey(table_name, concatKeys(key), value);
}