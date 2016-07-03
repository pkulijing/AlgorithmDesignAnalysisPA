#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
const int MAX_INDEX = 875714;
bool debug;
using std::vector;
using std::istream;
using std::ostream;
using std::cout;
using std::endl;
class DirectedGraph;
class SCC;
class QuickSort;
class QuickSort {
public:
	static void sort(vector<int>& nums) {
		sort(nums, 0, nums.size() - 1);
	}
private:
	static void sort(vector<int>& nums, int left, int right) {
		if(left >= right)
			return;
		int i = rand() % (right - left + 1) + left;
		int pos = partition(nums, left, right, i);
		sort(nums, left, pos - 1);
		sort(nums, pos + 1, right);
	}
	static int partition(vector<int>& nums, int left, int right, int i) {
		swap(nums, i, left);
		int fg = left + 1; // first greater element
		for(int fu = left + 1; fu <= right; fu++) { //fu: first unpartitioned element
			if(nums[fu] < nums[left]) {
				swap(nums, fu, fg++);
			}
		}
		swap(nums, fg - 1, left);
		return fg - 1;
	}
	static void swap(vector<int>& nums, int i, int j) {
		if(i == j)
			return;
		int temp = nums[i];
		nums[i] = nums[j];
		nums[j] = temp;
	}
};
class DirectedGraph {
public:
	DirectedGraph(istream& is, int max_index) : 
	edgeIn(max_index + 1), 
	edgeOut(max_index + 1),
	m_NumEdges(0) {
		int tail, head;
		while(is >> tail >> head) {
			edgeIn[head].push_back(tail);
			edgeOut[tail].push_back(head);
			m_NumEdges++;
		}
	}
	int numVertices() const { return edgeOut.size() - 1; }
	int64_t numEdges() const { return m_NumEdges; }

	const vector<int>& getOutEdgesOf(int i) const { return edgeOut[i]; }
	const vector<int>& getInEdgesOf(int i) const { return edgeIn[i]; }

	void print(ostream& os, bool printEdges = false, bool printInv = false) const {
		os << "number of vertices: " << numVertices() 
		   << "\nnumber of edges: " << numEdges() << endl;
		if(printEdges) {
			os << "Edges:\n";
			for(int i = 1; i <= numVertices(); ++i) {
				os << i << ":";
				if(printInv) {
					for(auto it = edgeIn[i].begin(); it != edgeIn[i].end(); ++it) {
						os << " " << *it;
					}
				} else {
					for(auto it = edgeOut[i].begin(); it != edgeOut[i].end(); ++it) {
						os << " " << *it;
					}
				}
				os << endl;
			}
		}
	}
private:
	int64_t m_NumEdges;
	vector<vector<int> > edgeIn;
	vector<vector<int> > edgeOut;
};
ostream& operator<<(ostream& os, const DirectedGraph& g) {
	g.print(os);
	return os;
}
class SCC {
public:
	SCC(const DirectedGraph& g) : 
	m_G(g),
	m_OrderIn2ndLoop(g.numVertices() + 1) {
		DFSLoop1();
		DFSLoop2();
	}

	void DFSLoop1() {
		vector<bool> explored(m_G.numVertices() + 1);
		int finishTime = 0;
		for(int i = m_G.numVertices(); i >= 1; i--) {
			if(!explored[i]) {
				DFS1(i, explored, finishTime);
			}
		}
		if(debug) {
			for(int i = 1; i <= m_G.numVertices(); i++) {
				cout << "m_OrderIn2ndLoop[" << i << "] = " << m_OrderIn2ndLoop[i] << endl;
			}
		}
	}
	void DFS1(int i, vector<bool>& explored, int& finishTime) {
		if(debug) 
			cout << "dealing with " << i << endl;
		explored[i] = true;
		const vector<int>& inEdges = m_G.getInEdgesOf(i);
		//use reverse iterator here to obtain same result as non recursion version
		for(auto it = inEdges.rbegin(); it != inEdges.rend(); ++it) {
			if(!explored[*it]) {
				DFS1(*it, explored, finishTime);
			}
		}
		m_OrderIn2ndLoop[++finishTime] = i;
	}
	void DFSLoop2() {
		vector<bool> explored(m_G.numVertices() + 1);
		for(int i = m_G.numVertices(); i >= 1; --i) {
			if(!explored[m_OrderIn2ndLoop[i]]) {
				m_SCCs.push_back(vector<int>());
				m_SCCSizes.push_back(0);
				DFS2(m_OrderIn2ndLoop[i], explored);
			}
		}
	}
	void DFS2(int i, vector<bool>& explored) {
		explored[i] = true;
		m_SCCs.back().push_back(i);
		m_SCCSizes.back()++;
		const vector<int>& outEdges = m_G.getOutEdgesOf(i);
		for(auto it = outEdges.begin(); it != outEdges.end(); ++it) {
			if(!explored[*it]) {
				DFS2(*it, explored);
			}
		}
	}
	void printSCCs(ostream& os) const {
		for(auto it = m_SCCs.begin(); it != m_SCCs.end(); ++it) {
			for(auto it1 = it->begin(); it1 != it->end(); ++it1) {
				os << " " << *it1;
			}
			os << "\n";
		}
	}
	void printSizes(ostream& os, int num) {
		os << m_SCCSizes.size() << " SCCs have been found. Largest " << num << " have sizes: \n"; 
		//The implementation of quick sort is correct but too slow when compared with the STL version.
		//QuickSort::sort(m_SCCSizes);
		std::sort(m_SCCSizes.begin(), m_SCCSizes.end());
		for(int i = 0; i < num; i++) {
			int index = m_SCCSizes.size() - 1 - i;
			os << (index >= 0 ? m_SCCSizes[index] : 0) << (i == num - 1 ? "\n" : ",");
		}
	}
private:
	const DirectedGraph& m_G;
	vector<int> m_OrderIn2ndLoop;
	vector<vector<int> > m_SCCs;
	vector<int> m_SCCSizes;

};
int main(int argc, char** argv) {
	std::srand(time(NULL));
	std::ifstream ifs(argv[1]);
	int max_size = argc > 2 ? std::atoi(argv[2]) : MAX_INDEX;
	debug = argc > 3 ? true : false;
	DirectedGraph g(ifs, max_size);
	g.print(cout);
	SCC scc(g);
	if(debug) 
		scc.printSCCs(cout);
	scc.printSizes(cout, 5);
	return 0;
}