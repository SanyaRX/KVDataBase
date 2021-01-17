#include "KVDataBase.h"

Database::Database() {}

Database::Database(string path) {
	this->dots = path;
}

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
	string path = dots + doubleslash + table_name + doubleslash + key;
	checkKeyExists(path);
	fs::remove_all(path);
}

//private
void Database::deleteValueByKey(string table_name, string key) {
	string path = dots + doubleslash + table_name + doubleslash + key;
	checkKeyExists(path);

	for (auto& p : fs::directory_iterator(path)) {
		string s = p.path().u8string();
		DeleteFile(wstring(s.begin(), s.end()).c_str());
		return;
	}
}

//private
vector<string> Database::getByKey(string table_name, string key) {
	string path = dots + doubleslash + table_name + doubleslash + key;
	checkKeyExists(path);
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
	checkKeyExists(path);
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


	result.pop_back();
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
	checkTableExists(table_name);
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
	checkTable(table_name);
	string s = dots + doubleslash + table_name + dottxt;
	DeleteFile(wstring(s.begin(), s.end()).c_str());
	return fs::remove_all(dots + doubleslash + table_name);
}

void Database::addValueByKeys(string table_name, vector<string> keys, string value) {
	checkTable(table_name);
	checkKey(table_name, keys);
	addValue(table_name, concatKeys(keys), value);
}

// do not delete folder as we can't add new value
void Database::deleteAllValuesByKeys(string table_name, vector<string> keys) {
	checkTable(table_name);
	checkKey(table_name, keys);
	checkTable(table_name);
	deleteAllValuesByKey(table_name, concatKeys(keys));
}

void Database::deleteValueByKeys(string table_name, vector<string> keys) {
	checkTable(table_name);
	checkKey(table_name, keys);
	checkTable(table_name);
	deleteValueByKey(table_name, concatKeys(keys));
}

string Database::getSortedByKey(string table_name, string key, bool isFirst) {
	checkTable(table_name);
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
	checkTable(table_name);
	checkKey(table_name, keys);
	return getByKey(table_name, concatKeys(keys));
}

void Database::updateByKeys(string table_name, vector<string> key, string value) {
	checkTable(table_name);
	checkKey(table_name, key);
	updateByKey(table_name, concatKeys(key), value);
}

vector<vector<string>> Database::getAllKeys(string table_name) {
	string path = dots + doubleslash + table_name;
	string erase_string = path + doubleslash;
	vector<vector<string>> all_keys;
	for (auto& p : fs::directory_iterator(path)) {
		vector<string> keys = split(p.path().u8string().c_str());
		eraseSubStr(keys[0], erase_string);
		all_keys.push_back(keys);
	}
	return all_keys;
}

vector<string>  Database::getFromEdge(string table_name, boolean isFirst) {
	checkTable(table_name);
	string path = dots + doubleslash + table_name;
	string erase_string = path + doubleslash;
	string key = "";
	for (auto& p : fs::directory_iterator(path)) {
		key = p.path().u8string();
		if (isFirst)
			break;
	}
	vector<string> keys = split(key.c_str());
	eraseSubStr(keys[0], erase_string);
	return keys;
}

vector<string> Database::getFirstKey(string table_name) {
	return getFromEdge(table_name, true);
}

vector<string> Database::getLastKey(string table_name) {
	return getFromEdge(table_name, false);
}

vector<string> Database::getPrev(string table_name, vector<string> key) {
	checkTable(table_name);
	checkKey(table_name, key);

	string pathConcat = dots + doubleslash + table_name + doubleslash + concatKeys(key);
	checkKeyExists(pathConcat);

	string path = dots + doubleslash + table_name;
	string erase_string = path + doubleslash;
	string prevPath = "";

	for (auto& p : fs::directory_iterator(path)) {
		if (p.path().u8string() == pathConcat) {
			if (prevPath == "") {
				prevPath = p.path().u8string();
			}
			break;
		}
		prevPath = p.path().u8string();
	}

	vector<string> keys = split(prevPath.c_str());
	eraseSubStr(keys[0], erase_string);
	return keys;
}

vector<string> Database::getNext(string table_name, vector<string> key) {
	checkTable(table_name);
	checkKey(table_name, key);
	string pathConcat = dots + doubleslash + table_name + doubleslash + concatKeys(key);
	checkKeyExists(pathConcat);

	string path = dots + doubleslash + table_name;
	string erase_string = path + doubleslash;
	string nextPath = "";
	bool getNext = false;

	for (auto& p : fs::directory_iterator(path)) {
		if (getNext) {
			nextPath = p.path().u8string();
			break;
		}
		if (p.path().u8string() == pathConcat) {
			getNext = true;
		}
	}

	if (nextPath == "") {
		nextPath = pathConcat;
	}

	vector<string> keys = split(nextPath.c_str());
	eraseSubStr(keys[0], erase_string);
	return keys;
}

void Database::checkKey(string table_name, vector<string> key) {
	ifstream myfile(dots + doubleslash + table_name + dottxt);
	int keys = 0;
	string line;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			keys++;
		}
		myfile.close();
	}
	if (keys != key.size())
		throw InvalidKeySizeException();
}

bool Database::checkDirectoryExists(string path_s) {
	fs::path path = path_s;
	return fs::exists(path);
}

void Database::checkTable(string table_name) {
	if (!checkDirectoryExists(dots + doubleslash + table_name)) {
		throw ThereIsNoSuchTableException();
	}
}

void Database::checkTableExists(string table_name) {
	if (checkDirectoryExists(dots + doubleslash + table_name)) {
		throw TableAlreadyExistsException();
	}
}

void Database::checkKeyExists(string key_path) {
	if (!checkDirectoryExists(key_path)) {
		throw ThereIsNoSuchKeyException();
	}
}