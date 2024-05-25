#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

struct Entry
{
    long timestamp;
    string errortype;
    float severity;
};

struct Data
{
    float sum = 0;
    float minSeverity = numeric_limits<float>::max();
    float maxSeverity = numeric_limits<float>::min();
    int count = 0;
};

struct Node
{
    int start, end;
    Node *left;
    Node *right;
    map<string, Data> errorToDataMap;

    Node(int start, int end) : start(start), end(end), left(nullptr), right(nullptr) {}
};

class SegmentTree
{
private:
    Node *root;

    Node *build(vector<Entry> &data, int start, int end)
    {
        if (start > end)
            return nullptr;

        Node *node = new Node(start, end);
        if (start == end)
        {
            Data temp;
            temp.sum = data[start].severity;
            temp.minSeverity = data[start].severity;
            temp.maxSeverity = data[start].severity;
            temp.count = 1;
            node->errorToDataMap[data[start].errortype] = temp;
        }
        else
        {
            int mid = (start + end) / 2;
            node->left = build(data, start, mid);
            node->right = build(data, mid + 1, end);

            for (const auto &pair : node->left->errorToDataMap)
            {
                node->errorToDataMap[pair.first].sum += pair.second.sum;
                node->errorToDataMap[pair.first].minSeverity = min(node->errorToDataMap[pair.first].minSeverity, pair.second.minSeverity);
                node->errorToDataMap[pair.first].maxSeverity = max(node->errorToDataMap[pair.first].maxSeverity, pair.second.maxSeverity);
                node->errorToDataMap[pair.first].count += pair.second.count;
            }

            for (const auto &pair : node->right->errorToDataMap)
            {
                node->errorToDataMap[pair.first].sum += pair.second.sum;
                node->errorToDataMap[pair.first].minSeverity = min(node->errorToDataMap[pair.first].minSeverity, pair.second.minSeverity);
                node->errorToDataMap[pair.first].maxSeverity = max(node->errorToDataMap[pair.first].maxSeverity, pair.second.maxSeverity);
                node->errorToDataMap[pair.first].count += pair.second.count;
            }
        }
        return node;
    }

    float query(Node *node, int L, int R, float &minSeverity, float &maxSeverity, int &count)
    {
        if (!node || node->start > R || node->end < L)
            return 0;
        if (L <= node->start && node->end <= R)
        {
            minSeverity = numeric_limits<float>::max();
            maxSeverity = numeric_limits<float>::min();
            float sum = 0;
            count = 0;
            for (const auto &pair : node->errorToDataMap)
            {
                sum += pair.second.sum;
                minSeverity = min(minSeverity, pair.second.minSeverity);
                maxSeverity = max(maxSeverity, pair.second.maxSeverity);
                count += pair.second.count;
            }
            return sum;
        }
        float leftMin, leftMax, rightMin, rightMax;
        int leftCount = 0, rightCount = 0;
        float leftSum = query(node->left, L, R, leftMin, leftMax, leftCount);
        float rightSum = query(node->right, L, R, rightMin, rightMax, rightCount);
        minSeverity = min(leftMin, rightMin);
        maxSeverity = max(leftMax, rightMax);
        count = leftCount + rightCount;
        return leftSum + rightSum;
    }

    float queryByErrorType(Node *node, int L, int R, const string &errorType, float &minSeverity, float &maxSeverity, int &count)
    {
        if (!node || node->start > R || node->end < L)
            return 0;
        if (L <= node->start && node->end <= R)
        {
            if (node->errorToDataMap.find(errorType) != node->errorToDataMap.end())
            {
                const Data &data = node->errorToDataMap[errorType];
                minSeverity = data.minSeverity;
                maxSeverity = data.maxSeverity;
                count = data.count;
                return data.sum;
            }
            else
            {
                minSeverity = numeric_limits<float>::max();
                maxSeverity = numeric_limits<float>::min();
                count = 0;
                return 0;
            }
        }
        float leftMin, leftMax, rightMin, rightMax;
        int leftCount = 0, rightCount = 0;
        float leftSum = queryByErrorType(node->left, L, R, errorType, leftMin, leftMax, leftCount);
        float rightSum = queryByErrorType(node->right, L, R, errorType, rightMin, rightMax, rightCount);
        minSeverity = min(leftMin, rightMin);
        maxSeverity = max(leftMax, rightMax);
        count = leftCount + rightCount;
        return leftSum + rightSum;
    }

    void deleteTree(Node *node)
    {
        if (node == nullptr)
            return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    SegmentTree(vector<Entry> &data)
    {
        root = build(data, 0, data.size() - 1);
    }

    ~SegmentTree()
    {
        deleteTree(root);
    }

    float query(int start, int end, float &minSeverity, float &maxSeverity, int &count)
    {
        return query(root, start, end, minSeverity, maxSeverity, count);
    }

    float queryByErrorType(int start, int end, const string &errorType, float &minSeverity, float &maxSeverity, int &count)
    {
        return queryByErrorType(root, start, end, errorType, minSeverity, maxSeverity, count);
    }

    void rebuild(vector<Entry> &data)
    {
        deleteTree(root);
        root = build(data, 0, data.size() - 1);
    }
};

int main()
{
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<Entry> data(n);
    cout << "Enter the elements: " << endl;
    for (int i = 0; i < n; ++i)
    {
        cout << "Enter timestamp: ";
        cin >> data[i].timestamp;
        cout << "Enter errortype: ";
        cin >> data[i].errortype;
        cout << "Enter severity: ";
        cin >> data[i].severity;
    }
    map<long, int> mp;
    for (int i = 0; i < n; ++i)
    {
        mp[data[i].timestamp] = i;
    }

    SegmentTree segTree(data);

    int queries;
    cout << "Enter number of queries: ";
    cin >> queries;

    for (int i = 0; i < queries; ++i)
    {
        int type;
        cout << "1. Add an entry\n2. Query for a logtype\n3. Query before a timestamp\n4. Query after a timestamp\n5. Query before a timestamp of a specific error type\n6. Query after a timestamp of a specific error type\n";
        cin >> type;
        if (type == 1)
        {
            Entry entry;
            cout << "Enter timestamp: ";
            cin >> entry.timestamp;
            cout << "Enter errortype: ";
            cin >> entry.errortype;
            cout << "Enter severity: ";
            cin >> entry.severity;

            data.push_back(entry);
            mp[entry.timestamp] = data.size() - 1;

            segTree.rebuild(data);
        }
        else if (type == 2){
            string errorType;
            cout<<"Enter errortype : ";
            cin>>errorType;

            float minSeverity, maxSeverity;
            int count = 0;
            float sum =segTree.queryByErrorType(0, data.size()-1, errorType, minSeverity, maxSeverity, count);
            cout << "Min: " << minSeverity << ", Max: " << maxSeverity << ", Mean: " << (count ? sum / count : 0) << endl;

        }
        else if (type == 3)
        {
            long ts;
            cout << "Enter timestamp: ";
            cin >> ts;

            auto it = lower_bound(mp.begin(), mp.end(), ts, [](const pair<long, int> &a, long b){ return a.first < b; });

            int pos = (it == mp.end()) ? data.size() - 1 : distance(mp.begin(), it);
            if (it != mp.end() && it->first > ts && it != mp.begin())
            {
                pos = distance(mp.begin(), prev(it));
            }

            float minSeverity, maxSeverity;
            int count = 0;
            float sum = segTree.query(0, pos, minSeverity, maxSeverity, count);
            cout << "Min: " << minSeverity << ", Max: " << maxSeverity << ", Mean: " << (count ? sum / count : 0) << endl;
        }
        else if (type == 4)
        {
            long ts;
            cout << "Enter timestamp: ";
            cin >> ts;

            auto it = upper_bound(mp.begin(), mp.end(), ts, [](long b, const pair<long, int> &a)  { return b < a.first; });

            int pos = (it == mp.end()) ? data.size() - 1 : distance(mp.begin(), it);

            float minSeverity, maxSeverity;
            int count = 0;
            float sum = segTree.query(pos, data.size() - 1, minSeverity, maxSeverity, count);
            cout << "Min: " << minSeverity << ", Max: " << maxSeverity << ", Mean: " << (count ? sum / count : 0) << endl;
        }
        else if (type == 5)
        {
            long ts;
            cout << "Enter timestamp: ";
            cin >> ts;
            string errorType;
            cout << "Enter errortype: ";
            cin >> errorType;

            auto it = lower_bound(mp.begin(), mp.end(), ts, [](const pair<long, int> &a, long b) { return a.first < b; });

            int pos = (it == mp.end()) ? data.size() - 1 : distance(mp.begin(), it);
            if (it != mp.end() && it->first > ts && it != mp.begin())
            {
                pos = distance(mp.begin(), prev(it));
            }

            float minSeverity, maxSeverity;
            int count = 0;
            float sum = segTree.queryByErrorType(0, pos, errorType, minSeverity, maxSeverity, count);
            cout << "Min: " << minSeverity << ", Max: " << maxSeverity << ", Mean: " << (count ? sum / count : 0) << endl;
        }
        else if (type == 6)
        {
            long ts;
            cout << "Enter timestamp: ";
            cin >> ts;
            string errorType;
            cout << "Enter errortype: ";
            cin >> errorType;

            auto it = upper_bound(mp.begin(), mp.end(), ts, [](long b, const pair<long, int> &a) { return b < a.first; });

            int pos = (it == mp.end()) ? data.size() - 1 : distance(mp.begin(), it);

            float minSeverity, maxSeverity;
            int count = 0;
            float sum = segTree.queryByErrorType(pos, data.size() - 1, errorType, minSeverity, maxSeverity, count);
            cout << "Min: " << minSeverity << ", Max: " << maxSeverity << ", Mean: " << (count ? sum / count : 0) << endl;
        }
    }

    return 0;
}
