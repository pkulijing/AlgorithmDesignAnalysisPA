#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;
static int compInt(int i, int j) {
	if(i > j) return 1;
	if(i < j) return -1;
	return 0;
}
struct Job {
	Job(int w, int l) : weight(w), length(l) {}
	int weight;
	int length;
	int compDiff(const Job& another) const {
		int diffRes = compInt(weight - length, another.weight - another.length);
		if(diffRes == 0) {
			return compInt(weight, another.weight);
		}
		return diffRes;
	}
	int compRatio(const Job& another) const {
		return compInt(weight * another.length, length * another.weight);
	}
};
template <typename T>
class Stack {
public:
	void push(const T& t) { items.push_back(t); }
	T pop() { 
		T res = items.back();
		items.resize(size() - 1);
		return res;
	}
	size_t size() const { return items.size(); }
	bool empty() const { return size() == 0; }
private:
	vector<T> items;
};
template <typename T>
class QuickSort {
public:
	QuickSort(int (T::*comp)(const T&) const) : compare(comp) {}
	void sort(vector<T>& items) {
		Stack<int> lefts, rights;
		lefts.push(0);
		rights.push(items.size() - 1);
		while(!lefts.empty()) {
			int left = lefts.pop();
			int right = rights.pop();
			int pivot = left + rand() % (right - left + 1);
			int pos = partition(items, left, right, pivot);
			if(left < pos - 1) {
				lefts.push(left);
				rights.push(pos - 1);	
			}
			if(pos + 1 < right) {
				lefts.push(pos + 1);
				rights.push(right);				
			}
		}
	};
private:
	static void swap(vector<T>& items, int i, int j) {
		if(i == j) return;
		T temp = items[i];
		items[i] = items[j];
		items[j] = temp;
	} 
	int partition(vector<T>& items, int left, int right, int pivot) {
		swap(items, left, pivot);
		int firstLarger = left + 1;
		for(int i = left + 1; i <= right; ++i) {
			if((items[i].*compare)(items[left]) > 0) {
				swap(items, i, firstLarger++);
			}
		}
		swap(items, firstLarger - 1, left);
		return firstLarger - 1;
	}	
	int (T::*compare)(const T&) const;
};
int64_t weightedSum(const vector<Job>& jobs) {
	int t = 0;
	int64_t sum = 0;
	for(auto it = jobs.begin(); it != jobs.end(); ++it) {
		t += it->length;
		sum += t * it->weight;
	}
	return sum;
}
int main(int argc, char** argv) {
	srand(time(NULL));
	ifstream ifs(argv[1]);
	int ct;
	ifs >> ct;
	vector<Job> jobs;
	for(int i = 0; i < ct; i++) {
		int w, l;
		ifs >> w >> l;
		jobs.push_back(Job(w, l));
	}
	vector<Job> jobsRatio = jobs, jobsDiff = jobs;
	QuickSort<Job> qsRatio(&Job::compRatio);
	QuickSort<Job> qsDiff(&Job::compDiff);
	qsRatio.sort(jobsRatio);
	cout << "ratio:\n";
	for(auto it = jobsRatio.begin(); it != jobsRatio.end(); ++it) {
		cout << it->weight << ", " << it->length << "\n";
	}
	cout << "diff:\n";
	qsDiff.sort(jobsDiff);
	for(auto it = jobsDiff.begin(); it != jobsDiff.end(); ++it) {
		cout << it->weight << ", " << it->length << "\n";
	}
	cout << "Ratio: " << weightedSum(jobsRatio) << ", Diff: " << weightedSum(jobsDiff) << endl;
	return 0;
}