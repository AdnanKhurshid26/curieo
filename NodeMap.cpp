#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include "Data.cpp"


using namespace std;

class NodeMap {
public:
    unordered_map<string, Data> nodeMap;


    NodeMap() {}

     NodeMap* getUpdatedNodeMap(const NodeMap& nmap) {
        for (const auto& node : nmap.nodeMap) {
            addData(node.first, node.second);
        }
        return this;
    }


    vector<Data> getDataList() const {
        vector<Data> nodeList;
        for (const auto& node : nodeMap) {
            nodeList.push_back(node.second);
        }
        return nodeList;
    }

    void addData(const string& errortype, const Data& data) {
        if (!nodeMap.count(errortype)) {
            nodeMap[errortype] = Data(data);
        } else {
            Data& node = nodeMap[errortype];
            node.getUpdatedData(data);
        }
    }


const Data* getDataByKey(const string& key) const {
    if (nodeMap.count(key)) {
        return &nodeMap.at(key);
    }
    return nullptr;
}

    bool isEmpty() const {
        return nodeMap.empty();
    }

};
