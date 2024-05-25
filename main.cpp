#include <iostream>
#include <vector>
#include <map>
#include <limits>

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
    map<long, int> mp;

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

        mp[data[i].timestamp] = i;
    }

    SegmentTree segTree(data);

    int queries;
    cout << "Enter number of queries: ";
    cin >> queries;

    for (int i = 0; i < queries; ++i)
    {
        int type;
        cout << "Enter query type (1 for range sum, 2 to add an element): ";
        cin >> type;
        if (type == 1)
        {
            int ts;
            cout << "Enter timestamp: ";
            cin >> ts;
            float minSeverity, maxSeverity;
            int count = 0;
            float sum = segTree.query(0, mp[ts], minSeverity, maxSeverity, count);
            cout << "Min: " << minSeverity << ", Max: " << maxSeverity << ", Mean: " << (count ? sum / count : 0) << endl;
        }
        else if (type == 2)
        {
            Entry entry;
            cout << "Enter timestamp: ";
            cin >> entry.timestamp;
            cout << "Enter errortype: ";
            cin >> entry.errortype;
            cout << "Enter severity: ";
            cin >> entry.severity;
            data.push_back(entry);
            mp[entry.timestamp] = n++;
            segTree.rebuild(data);
        }
        else
        {
            cout << "Invalid query type!" << endl;
        }
    }

    return 0;
}
