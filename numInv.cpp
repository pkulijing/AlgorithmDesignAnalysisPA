//calculate number of inversions in an array.
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
using namespace std;
int64_t numInv(vector<int>& nums, vector<int>& buffer, int begin, int end) {
	if(end - begin <= 1) 
		return 0;
	int middle = (begin + end) / 2;
	int64_t invLeft = numInv(nums, buffer, begin, middle);
	int64_t invRight = numInv(nums, buffer, middle, end);
	int ileft = begin, iright = middle;
	//It has to be int64_t! Int shall overflow in the test case. unsigned int is not safe in the worst case,
	//in which there are 100000*99999/2 > 2^32 inversions.
	int64_t invSplit = 0;
	for(int k = begin; k < end; k++) {
		if(ileft == middle) {
			buffer[k] = nums[iright++];
		} else if (iright == end) {
			buffer[k] = nums[ileft++];
		} else if(nums[ileft] < nums[iright]) { 
			buffer[k] = nums[ileft++];
		} else {
			buffer[k] = nums[iright++];
			invSplit += middle - ileft; 
		}
	}
	for(int k = begin; k < end; k++) {
		nums[k] = buffer[k];
	}
	return invLeft + invRight + invSplit;
}

int64_t numInv(vector<int>& nums) {
	vector<int> buffer(nums.size());
	return numInv(nums, buffer, 0, nums.size());
}

int main(int argc, char **argv) {
	vector<int> nums;
	int temp;

	ifstream ifs(argv[1]);
	while(ifs >> temp)
		nums.push_back(temp);
	
	cout << "size = " << nums.size() << endl;
	auto before_time = chrono::system_clock::now();	
	cout << "number of inversions = " << numInv(nums) << endl;
	auto after_time = chrono::system_clock::now();	
	cout << "It took me " << chrono::duration_cast<chrono::milliseconds>(after_time - before_time).count()
		<< " ms to calculate." << endl;
	return 0;
}