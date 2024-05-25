# **Approach**

The problem seems like we would have to do a lot of range queries and the ranges can be very big since the timestamp is a 64 bit integer so  I used Segment Tree which has Logarithmic time complexity for range queries.

Also there are queries with specific error type so it is needed to have nodes categorized in the segment tree for all the error types. Instead of creating a separate segment tree for every error type, there is a map at every node which holds the data for all error types at that node (basically in a range )

The code can handle multiple log entries at same timestamp as well. If different error type comes at the same timestamp then it is added to the map else if the error type already exists then it is simply cumulated to the Data of the exisitng error type at that node

The range for the segment tree is initialised for the range 0 to maximum value of long long data type but it does not have any nodes created so no space is wasted. The log entries are inserted at runtime according to their timestamp and the parent nodes are updated while backtracking. First midpoint of the range is calculated at every node then depending on whether the incoming timestamp is smaller or greater, the entry is inserted to left or right of current node. If there is no node in place , then a node is created else the entry is inserted into the map of the determined node.

In earlier appraoch, the timestamp was mapped to an increasing index and the tree was rebuilt for every insertion which was then scrapped because insertion was taking linear time instead of logarithmic.

#### **Data Structures Used :** 

Segment Tree 

Unordered map

#### There are several classes :

Data : It holds the data (maximum severity, minimum severity, sum , count) for a specific error type in a range

NodeMap : This contains a map which has the keys as errortypes and values as Data objects and has member functions for operating on it.

SegTree : This is a segment tree node which has start of range, end of range, a nodemap  and left and right pointers to its children nodes.  It has member functions to add new log entries to Nodes depending on the errortype at runtime and querying ranges

Parser : It has helper methods to parse queries and return tokens 

LogMonitor : It initializes a segment tree then reads a file for queries and parses , then depending on type of query : inserts or returns a response

## Time complexity:

The insertiom basically works like divide and conquer so the time complexity is approximately O(logn)

Query : O(logn)


# How to Run

There is a python script run.py which takes two file paths as arguments : "input.txt" and "output.txt"


Simply write the queries in input.txt file  and then run the below comman on your terminal

### `python run.py input.txt output.txt                           `

NOTE : There is also a dockerfile and a shell script to execute the program as a docker image. The python script was written to easily dockerize the program but due to time constraints it could not be dockerized correctly.
