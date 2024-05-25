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

using namespace std;

class Data
{
public:
    float minSeverity;
    float maxSeverity;
    float sum;
    int count;

    Data() : minSeverity(numeric_limits<float>::max()), maxSeverity(numeric_limits<float>::min()), sum(0), count(0) {}

    Data(float min, float max, float sum, int count) : minSeverity(min), maxSeverity(max), sum(sum), count(count) {}

    Data* getUpdatedData(const Data &data)
    {
        minSeverity = min(minSeverity, data.minSeverity);
        maxSeverity = max(maxSeverity, data.maxSeverity);
        sum += data.sum;
        count += data.count;
        return this;
    }
};
