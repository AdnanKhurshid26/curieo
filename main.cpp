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

struct Node
{
    int start, end;
    float sum;
    float minSeverity;
    float maxSeverity;
    Node *left;
    Node *right;

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
            node->sum = data[start].severity;
            node->minSeverity = data[start].severity;
            node->maxSeverity = data[start].severity;
        }
        else
        {
            int mid = (start + end) / 2;
            node->left = build(data, start, mid);
            node->right = build(data, mid + 1, end);
            node->sum = (node->left ? node->left->sum : 0) + (node->right ? node->right->sum : 0);
            node->minSeverity = min((node->left ? node->left->minSeverity : numeric_limits<float>::max()), (node->right ? node->right->minSeverity : numeric_limits<float>::max()));
            node->maxSeverity = max((node->left ? node->left->maxSeverity : numeric_limits<float>::min()), (node->right ? node->right->maxSeverity : numeric_limits<float>::min()));
        }
        return node;
    }

    int query(Node *node, int L, int R, float &minSeverity, float &maxSeverity)
    {
        if (node->start > R || node->end < L)
            return 0;
        if (L <= node->start && node->end <= R)
        {
            minSeverity = node->minSeverity;
            maxSeverity = node->maxSeverity;
            return node->sum;
        }
        float leftMin, leftMax, rightMin, rightMax;
        int leftSum = query(node->left, L, R, leftMin, leftMax);
        int rightSum = query(node->right, L, R, rightMin, rightMax);
        minSeverity = min(leftMin, rightMin);
        maxSeverity = max(leftMax, rightMax);
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

    int query(int start, int end, float &minSeverity, float &maxSeverity)
    {
        return query(root, start, end, minSeverity, maxSeverity);
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
            float sum = segTree.query(0, mp[ts], minSeverity, maxSeverity);
            cout << "Min: " << minSeverity<<", Max: "<<maxSeverity<<", Mean: "<<(float)sum / (mp[ts] + 1) <<endl;
        }
        else if (type == 2)
        {
            Entry Entry;
            cout << "Enter timestamp: ";
            cin >> Entry.timestamp;
            cout << "Enter errortype: ";
            cin >> Entry.errortype;
            cout << "Enter severity: ";
            cin >> Entry.severity;
            data.push_back(Entry);
            mp[Entry.timestamp] = n++;
            segTree.rebuild(data);
        }
        else
        {
            cout << "Invalid query type!" << endl;
        }
    }

    return 0;
}
