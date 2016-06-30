#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
void output(const vector<int>& a) {
	for(int i = 0; i < a.size(); i++) {
		cout << a[i] << ' ';
	}
	cout << endl;
}
enum PivotIndex {
	FIRST,
	LAST,
	MEDIAN
};
class QuickSort {
public:
QuickSort(PivotIndex pi = MEDIAN): m_PivotIndex(pi) {}
int sort(vector<int>& a) {
	if(m_PivotIndex == FIRST)
		return sortFirst(a, 0, a.size() - 1);
	else if(m_PivotIndex == LAST)
		return sortLast(a, 0, a.size() - 1);
	else
		return sortMedian(a, 0, a.size() - 1);
}
static bool checkSorted(const vector<int>& a) {
	for(int i = 0; i < a.size() - 1; i++) {
		if(a[i] > a[i+1])
			return false;
	}
	return true;
}
private:
PivotIndex m_PivotIndex;

static int partition(vector<int>& a, int ileft, int iright, int ipivot) {
	//swap pivot and 1st element
	swap(a, ileft, ipivot);
	int firstLarger = ileft + 1;
	for(int i = ileft + 1; i <= iright; i++) {
		if(a[i] < a[ileft]) {
			swap(a, firstLarger++, i);
		}
	}
	swap(a, ileft, firstLarger - 1);
	return firstLarger - 1;
}

static int sortFirst(vector<int>& a, int ileft, int iright) {
	if(ileft >= iright)
		return 0;
	int pivotPos = partition(a, ileft, iright, ileft);
	return iright - ileft + sortFirst(a, ileft, pivotPos - 1) +
		sortFirst(a, pivotPos + 1, iright);
}
static int sortLast(vector<int>& a, int ileft, int iright) {
	if(ileft >= iright)
		return 0;
	int pivotPos = partition(a, ileft, iright, iright);
	return iright - ileft + sortLast(a, ileft, pivotPos - 1) +
		sortLast(a, pivotPos + 1, iright);
}
static int sortMedian(vector<int>& a, int ileft, int iright) {
	if(ileft >= iright)
		return 0;
	int pivotPos = partition(a, ileft, iright, median(a, ileft, iright));
	return iright - ileft + sortMedian(a, ileft, pivotPos - 1) +
		sortMedian(a, pivotPos + 1, iright);
}
static int median(const vector<int>& a, int ileft, int iright) {
	int imiddle = ileft + (iright - ileft) / 2;
	int x = a[ileft] - a[imiddle];
	int y = a[imiddle] - a[iright];
	int z = a[iright] - a[ileft];
	int res = ileft;
	if(x * y >= 0)
		res = imiddle;
	else if (y * z > 0)
		res = iright; 
	return res;
}
static void swap(vector<int>& a, int i, int j) {
	if(i == j)
		return;
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}
};
int main(int argc, char **argv) {
	QuickSort qs1(FIRST), qs2(LAST),qs3(MEDIAN);

	vector<int> a;
	int temp;
	ifstream ifs(argv[1]);
	while(ifs >> temp)
		a.push_back(temp);
	vector<int> b = a, c = a;

	int num1 = qs1.sort(a);
	cout << (QuickSort::checkSorted(a) ? "a is sorted with " + to_string(num1) + " comparisons." : "a is not sorted" ) << endl;

	int num2 = qs2.sort(b);
	cout << (QuickSort::checkSorted(b) ? "b is sorted with " + to_string(num2) + " comparisons." : "b is not sorted" ) << endl;

	int num3 = qs3.sort(c);
	cout << (QuickSort::checkSorted(c) ? "c is sorted with " + to_string(num3) + " comparisons." : "c is not sorted" ) << endl;

	return 0;
}