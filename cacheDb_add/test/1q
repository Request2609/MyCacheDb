#include <iostream>
#include <unordered_map>
using namespace std ;
struct KEY {

	int first;
	string second;
	int third;
 
	KEY(int f, int s, int t) : first(f), second("aa"), third(t){}
};

struct HashFunc
{
	std::size_t operator()(const KEY &key) const 
	{
		using std::size_t;
		using std::hash;
 
		return ((hash<int>()(key.first)
			^ (hash<string>()(key.second) << 1)) >> 1)
			^ (hash<int>()(key.third) << 1);
	}
};
struct EqualKey
{
	bool operator () (const KEY &lhs, const KEY &rhs) const
	{
		return lhs.first  == rhs.first
			&& lhs.second == rhs.second
			&& lhs.third  == rhs.third;
	}
};
int main()
{
	unordered_map<KEY, string, HashFunc, EqualKey> hashmap =
	{
		{ { 01, 02, 03 }, "one" },
		{ { 11, 12, 13 }, "two" },
		{ { 21, 22, 23 }, "three" },
	};

	KEY key(11, 12, 13);

	auto it = hashmap.find(key);

 	if (it != hashmap.end())
 	{
 		cout << it->second << endl;
 	}

	return 0;
}

