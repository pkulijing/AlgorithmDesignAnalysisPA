#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>
#include <set>
using namespace std;
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
int main_2(int argc, char** argv) {
	ifstream ifs(argv[1]);
	vector<bool> check(20001,false);
	multiset<int64_t> nums;
	int64_t num;
	while(ifs >> num) {
		nums.insert(num);
	}
	cout << nums.size() << endl;
	int s = 0;
	for(int64_t i = -10000; i <= 10000; i++) {
		for(auto it = nums.begin(); it != nums.end(); ++it) {
			int64_t res = i - *it;
			if(nums.find(res) == nums.end())
				continue;
			if(res == *it && nums.count(res) == 1)
				continue;

			check[i + 10000] = true;
			s++;
			break;
		}
		cout << i << (check[i + 10000] ? " found" : " not found");
		cout << ". s = " << s << endl;
	}
	cout << s << endl;
	return 0;
}
int main(int argc, char** argv) {
	return main_1(argc, argv);
}