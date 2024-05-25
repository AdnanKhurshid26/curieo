#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include<unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <filesystem>

using namespace std;

namespace fs = filesystem;


class Parser{
    public:
    static vector<string> parseInsertQuery(const string& s) {
        istringstream iss(s);
        vector<string> tokens;
        string token;
        while (getline(iss, token, ';')) {
            tokens.push_back(token);
        }
        return tokens;
    }
    static vector<string> splitBySpace(const string& query) {
        istringstream iss(query);
        vector<string> tokens;
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
};
