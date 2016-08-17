#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>
#include <set>
#include <cstdio>
#include <ctime>
using namespace std;
//This is a super fast approach without using hash
pair<bool,int> locate(int64_t i, const vector<int64_t>& sorted) {
	int first = 0, last = sorted.size() - 1;
	while(last >= first) {
		int middle = first + (last - first) / 2;
		if(sorted[middle] == i) 
			return make_pair(true,middle);
		else if(sorted[middle] > i)
			last = middle - 1;
		else//sorted[middle] < i
			first = middle + 1;
	}
	return make_pair(false,last);//the largest element smaller than i 
}
int main_1(int argc, char** argv) {
	ifstream ifs(argv[1]);
	vector<bool> check(20001,false);
	vector<int64_t> nums;
	nums.reserve(1000000);
	int64_t num;
	while(ifs >> num) {
		nums.push_back(num);
	}
	cout << nums.size() << endl;
	vector<int64_t> sorted = nums;
	std::sort(sorted.begin(),sorted.end());
	int s = 0;
	for(int j = 0; j < sorted.size(); j++) {
		auto locate1 = locate(-10000 - sorted[j], sorted);
		auto locate2 = locate(10000 - sorted[j], sorted);
		int pos1 = (locate1.first ? locate1.second : locate1.second + 1);
		while(pos1 > 0 && sorted[pos1 - 1] == -10000 - sorted[j]) {
			pos1--;
		}
		int pos2 = locate2.second;
		while(pos2 < sorted.size() - 1 && sorted[pos2 + 1] == 10000 - sorted[j]) {
			pos2++;
		}
		for(int i = pos1; i <= pos2; i++) {
			if(i == j)
				continue;				
			int sum = sorted[j] + sorted[i];
			assert(sum >= -10000 && sum <= 10000);
			check[10000 + sum] = true;
		}
	}
	for(int i = 0; i < check.size(); i++)
		if(check[i])
			s++;
	cout << s << endl;
	return 0;
}
//This is a super fast approach using hash.
class Hash {
public:
	Hash(size_t range) : _range(range), _content(range * 2 - 1) {}
	void add(int64_t i) { _content[hash(i)].push_back(i); }
	int count(int64_t i) const {
		int res = 0;
		const vector<int64_t>& nums = _content[hash(i)];
		for(auto it = nums.begin(); it != nums.end(); ++it) {
			if(*it == i)
				res++;
		}
		return res;
	}
	bool check(int64_t i) const {
		const vector<int64_t>& nums = _content[hash(i)];
		for(auto it = nums.begin(); it != nums.end(); ++it) {
			if(*it == i) return true;
		}
		return false;
	}
	const vector<int64_t>& getSetWithHash(int i) const { return _content[i]; }
	size_t range() const { return _range; }
	//This is a clever hash function for this problem!
  	int hash(int64_t i) const { return (i / 10000) % _range + _range; }
private:
	vector<vector<int64_t> > _content;
	const size_t _range;
};

void solve(const Hash& hashTable, int iTarget, int64_t num, vector<bool>& check) {
	auto& target = hashTable.getSetWithHash(iTarget);
	for(auto it = target.begin(); it != target.end(); ++it) {
		int64_t sum = *it + num;
		if(sum >= -10000 && sum <= 10000) {
			if(*it != num || hashTable.count(num) > 1)
				check[sum + 10000] = true;
		}
	}
}
int main_2(int argc, char** argv) {
	ifstream ifs(argv[1]);
	vector<bool> check(20001,false);
	vector<int64_t> nums;
	int64_t num;
	while(ifs >> num) {
		nums.push_back(num);
	}
	cout << nums.size() << endl;
	Hash hashTable(1000000);
	cout << "constructed a hash table of range " << hashTable.range() << endl;
	for(auto it = nums.begin(); it != nums.end(); ++it) {
		hashTable.add(*it);
	}
	cout << "added all numbers into the table." << endl;  
	for(auto it = nums.begin(); it != nums.end(); ++it) {
		int hashi = hashTable.hash(*it);
		solve(hashTable, -hashi + 1 + 2 * hashTable.range(), *it, check);
		solve(hashTable, -hashi 	 + 2 * hashTable.range(), *it, check);
		solve(hashTable, -hashi - 1 + 2 * hashTable.range(), *it, check);
		solve(hashTable, -hashi - 2 + 2 * hashTable.range(), *it, check);
	}

	int s = 0;
	for(int i = 0; i < check.size(); i++)
		if(check[i])
			s++;
	cout << s << endl;
	return 0;
}
int main(int argc, char** argv) {
	time_t curTime;
	time(&curTime);
	printf("%s", ctime(&curTime));
	main_1(argc, argv);
	time(&curTime);
	printf("%s", ctime(&curTime));
	main_2(argc, argv);
	time(&curTime);
	printf("%s", ctime(&curTime));
	return 0;
}