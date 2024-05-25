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
#include "SegTree.cpp"
#include "QueryParser.cpp"

using namespace std;

namespace fs = filesystem;

const float EPSILON = 1e-6;
const long long MAXTIMESTAMP = numeric_limits<long long>::max() - 1;
const float FLOAT_MAX = numeric_limits<float>::max();
const float FLOAT_MIN = numeric_limits<float>::min();

class LogMonitor
{
public:
    SegTree segTree;

    LogMonitor() : segTree(0, MAXTIMESTAMP) {}

    string calculateQueryResponse(const Data &node)
    {
        if (abs(node.maxSeverity - FLOAT_MIN) < EPSILON)
            return "Min: 0.0 Max: 0.0 Mean 0.0";

        float average = node.sum / node.count;
        return "Min: " + to_string(node.minSeverity) + " Max: " + to_string(node.maxSeverity) + " Mean " + to_string(average);
    }

    string calculateResponseForNode(const NodeMap &node)
    {
        if (node.isEmpty())
            return "Min: 0.0 Max: 0.0 Mean 0.0";

        float maxSeverity = FLOAT_MIN;
        float minSeverity = FLOAT_MAX;
        float sum = 0;
        int count = 0;

        for (const auto &dataNode : node.getDataList())
        {
            maxSeverity = max(maxSeverity, dataNode.maxSeverity);
            minSeverity = min(minSeverity, dataNode.minSeverity);
            sum += dataNode.sum;
            count += dataNode.count;
        }

        float average = sum / count;
        return "Min: " + to_string(minSeverity) + " Max: " + to_string(maxSeverity) + " Mean " + to_string(average);
    }

    string insert(const string &query)
    {
        vector<string> p = Parser::parseInsertQuery(query);
        long long timestamp = stoll(p[0]);
        string errortype = p[1];
        float severity = stof(p[2]);

        segTree.insert(timestamp, errortype, severity);
        return "No output";
    }

    string query(string &query)
    {
        vector<string> p = Parser::splitBySpace(query);

        if (p.size() == 1)
        {
            string errortype = p[0];
            Data aggregate = segTree.queryByErrorType(0, MAXTIMESTAMP, errortype);
            return calculateQueryResponse(aggregate);
        }

        if (p.size() == 2)
        {
            NodeMap nmap;

            string id = p[0];
            long long timestamp = stoll(p[1]);

            if (id == "BEFORE")
                nmap = segTree.queryAll(0, timestamp - 1);
            else
                nmap = segTree.queryAll(timestamp + 1, MAXTIMESTAMP);

            return calculateResponseForNode(nmap);
        }

        if (p.size() == 3)
        {
            Data d;

            string id = p[0];
            long long timestamp = stoll(p[2]);
            string errortype = p[1];

            if (id == "BEFORE")
                d = segTree.queryByErrorType(0, timestamp - 1, errortype);
            else
                d = segTree.queryByErrorType(timestamp + 1, MAXTIMESTAMP, errortype);

            return calculateQueryResponse(d);
        }

        return "";
    }

    const string queryHandler(const string &q)
    {
        char id = q[0];
        string actualQuery = q.substr(2);
        if (id == '1')
            return insert(actualQuery);
        else
            return query(actualQuery);
    }

    void writeToFile(vector<string> &responses, const string &outFilePath)
    {
        ofstream outFile(outFilePath);
        for (const string &r : responses)
        {
            outFile << r << endl;
        }
    }

    void execute(const string &filePath, const string &outFilePath)
    {
        ifstream inFile(filePath);
        vector<string> queries;
        string line;

        while (getline(inFile, line))
        {
            queries.push_back(line);
        }

        inFile.close();

        vector<string> responses;

        for (string &query : queries)
        {
            responses.push_back(queryHandler(query));
        }

        writeToFile(responses, outFilePath);
    }
};

int main(int argc, char* argv[])
{
     if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    string inputFilePath = argv[1];
    string outputFilePath = argv[2];

    LogMonitor LogMonitor;

    LogMonitor.execute(inputFilePath, outputFilePath);

    return 0;
}
