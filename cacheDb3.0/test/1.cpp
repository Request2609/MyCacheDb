#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>

#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;

//改变这个启用不同的hash方案
#define  RECORD_NAMESPACE

struct Record
{
    string name;
    int val;
};

#ifdef RECORD_FUNCTION_OBJECT
struct RecordHash
{
    size_t operator()(const Record& rhs) const{
        return hash<string>()(rhs.name) ^ hash<int>()(rhs.val);
    }
};
struct RecordCmp
{
    bool operator()(const Record& lhs, const Record& rhs) const{
        return lhs.name == rhs.name && lhs.val == rhs.val;
    }
};
unordered_set<Record, RecordHash, RecordCmp> records = {
    { "b", 100 }, { "a", 80 }, { "cc", 70 }, { "d", 60 }, { "d", 60 }
};
#endif//RECORD_FUNCTION_OBJECT

#ifdef RECORD_C_FUNCTION
size_t RecordHash(const Record& rhs){
    return hash<string>()(rhs.name) ^ hash<int>()(rhs.val);
}
bool RecordCmp(const Record& lhs, const Record& rhs){
    return lhs.name == rhs.name && lhs.val == rhs.val;
}
//直接使用成员初始化列表，vs2013不能编译通过
unordered_set<Record, decltype(&RecordHash), decltype(&RecordCmp)> records = {
    10,
    RecordHash, RecordCmp
};
struct RunBeforeMain
{
    RunBeforeMain(){
        records.insert({ "a", 100 });
    }
};
static RunBeforeMain dummyObject;
#endif //RECORD_C_FUNCTION

#ifdef RECORD_LAMBDA
//直接使用auto RecordHash不能编译通过，vs2013
auto &RecordHash = [](const Record& rhs){
    return hash<string>()(rhs.name) ^ hash<int>()(rhs.val);
};
auto &RecordCmp = [](const Record& lhs, const Record& rhs){
    return lhs.name == rhs.name && lhs.val == rhs.val;
};
unordered_set<Record, decltype(RecordHash), decltype(RecordCmp)> records = {
    10,
    RecordHash, RecordCmp
};
struct RunBeforeMain
{
    RunBeforeMain(){
        records.insert({ "a", 100 });
    }
};
static RunBeforeMain dummyObject;
#endif//RECORD_LAMBDA

#ifdef RECORD_NAMESPACE
namespace std{
    template<>
    struct hash<Record>
    {
        size_t operator()(const Record& rhs) const{
            return hash<string>()(rhs.name) ^ hash<int>()(rhs.val);
        }
    };

    template<>
    struct equal_to < Record > {
        bool operator()(const Record& lhs, const Record& rhs) const{
            return lhs.name == rhs.name && lhs.val == rhs.val;
        }
    };
}

unordered_set<Record> records = {
    { "b", 100 }, { "a", 80 }, { "cc", 70 }, { "d", 60 }, { "d", 60 }
};
#endif //RECORD_NAMESPACE


int main()
{
    auto showRecords = [](){
        for (auto i : records)
        {
            cout << "{" << i.name << "," << i.val << "}" << endl;
        }
    };
    showRecords();
    return 0;
}

