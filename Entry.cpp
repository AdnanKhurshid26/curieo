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

class Entry
{
public:
    long long timestamp;
    string errortype;
    float severity;

    Entry(long long ts, string et, float s) : timestamp(ts), errortype(et), severity(s) {}

};
