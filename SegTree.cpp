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
#include "NodeMap.cpp"

using namespace std;


class SegTree {
public:
    long long  start;
    long long end;
    NodeMap nmap;
    SegTree* left;
    SegTree* right;

    SegTree(long long start, long long end) : start(start), end(end), left(nullptr), right(nullptr) {}


    void insert(long long ts, const string& key, float severity) {
        Data newNode(severity, severity, severity, 1);
        if (start == end) {
            nmap.addData(key, newNode);
            return;
        }

        long long mid = start + (end - start) / 2;

        if (ts <= mid) {
            if (!left)
                left = new SegTree(start, mid);
            left->insert(ts, key, severity);
        } else {
            if (!right)
                right = new SegTree(mid + 1, end);
            right->insert(ts, key, severity);
        }
        nmap.addData(key, newNode);
    }

    void queryAllErrorTypes(long long l, long long r, NodeMap& result) const {
        if (l > r)
            return;

        if (start == l && end == r) {
            result.getUpdatedNodeMap(nmap);
            return;
        }

        long long mid = start + (end - start) / 2;

        if (left)
            left->queryAllErrorTypes(l, min(r, mid), result);

        if (right)
            right->queryAllErrorTypes(max(l, mid + 1), r, result);
    }

    NodeMap queryAll(long long l, long long r) const {
        NodeMap result;
        queryAllErrorTypes(l, r, result);
        return result;
    }

    void queryErrorType(long long l, long long r, const string& key, Data& result) const {
        if (l > r)
            return;

        if (start == l && end == r) {
            auto singleNode = nmap.getDataByKey(key);
            if (singleNode)
                result.getUpdatedData(*singleNode);
            return;
        }

        long long mid = start + (end - start) / 2;

        if (left)
            left->queryErrorType(l, min(r, mid), key, result);

        if (right)
            right->queryErrorType(max(l, mid + 1), r, key, result);
    }

    Data queryByErrorType(long long l, long long r, const string& key) const {
        Data result;
        queryErrorType(l, r, key, result);
        return result;
    }
};
