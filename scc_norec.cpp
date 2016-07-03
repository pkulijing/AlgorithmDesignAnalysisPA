#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <list>
#include <algorithm>
const int MAX_INDEX = 875714;
bool debug;
using std::vector;
using std::istream;
using std::ostream;
using std::cout;
using std::endl;
using std::list;
class DirectedGraph;
class SCC;
class QuickSort;
template <typename T>
class Stack {
public:
	Stack() {}
	void push(const T& t) { _content.push_back(t); }
	T pop() { 
		assert(_content.size() > 0);
		T val = _content.back();
		_content.resize(_content.size() - 1);
		return val;
	}
	T top() const { return _content.top(); }
	bool empty() const { return _content.empty(); }
	size_t size() const { return _content.size(); }
private:
	vector<T> _content;
};
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

	//Recursion -> iteration. Quite hard because this one is not tail recursion.
	void DFSLoop1() {
		vector<bool> explored(m_G.numVertices() + 1);
		Stack<int> vertices;
		int finishTime = 0;

		//Order by which the vertices get pushed on the stack.
		vector<int> order;

		//If a vertex a is pushed on the stack when processing vertex b because of the edge b->a,
		//b is a parent of a. A vertex can have multiple parents. A parent expects its sons to finish before
		//it finishes itself.
		vector<vector<int> > parents(m_G.numVertices() + 1);
		
		//Number of sons a vertex has. No need to remember them, just the number is sufficient.
		vector<int> numSons(m_G.numVertices() + 1);

		for(int i = m_G.numVertices(); i >= 1; i--) {
			if(!explored[i]) {
				vertices.push(i);
				while(!vertices.empty()) {
					int v = vertices.pop();
					//It is possible that a vertex gets added more than once. For example, if we have
					//7->4, 7->5 and 5->4, 4 will be added twice, and it's the one added by 5->4 
					// that counts. 
					if(explored[v])
						continue;
					if(debug)
						cout << "dealing with " << v << endl;
					//Popped a new vertex.
					order.push_back(v);
		 			explored[v] = true;
					const vector<int>& inEdges = m_G.getInEdgesOf(v);
					for(auto it = inEdges.begin(); it != inEdges.end(); ++it) {
						if(!explored[*it]) {
							vertices.push(*it);
							//update parents and numSons
							parents[*it].push_back(v);
							numSons[v]++;
						}
					}

					//vertices must finish in reverse order as they were popped out of the stack.
					//t is the number of vertices that finish here.
					int t = 0;
					//numSons[*it] = 0 means it should finish here
					for(auto it = order.rbegin(); it != order.rend() && numSons[*it] == 0; ++it) {
						//When a vertex finishes, decrease numSons[] of its parents by one.
						for(auto it1 = parents[*it].begin(); it1 != parents[*it].end(); ++it1)
							numSons[*it1]--;
						//*it finishes. Record its finish time
						m_OrderIn2ndLoop[++finishTime] = *it;
						++t;
					}
					//Discard vertices that already finish.
					order.resize(order.size() - t);
				}		
			}
		}
		if(debug) {
			for(int i = 1; i <= m_G.numVertices(); i++) 
				cout << "m_OrderIn2ndLoop[" << i << "] = " << m_OrderIn2ndLoop[i] << endl;
		}
	}
		
	//Recursion -> iteration. Not hard because this one is tail recursion. Just use a stack.
	void DFSLoop2() {
		vector<bool> explored(m_G.numVertices() + 1);
		Stack<int> vertices;

		for(int i = m_G.numVertices(); i >= 1; --i) {
			if(!explored[m_OrderIn2ndLoop[i]]) {
				m_SCCs.push_back(vector<int>());
				m_SCCSizes.push_back(0);
				vertices.push(m_OrderIn2ndLoop[i]);
				while(!vertices.empty()) {
					int v = vertices.pop();
					//It is possible that a vertex gets added more than once. For example, if we have
					//7->4, 7->5 and 5->4, 4 will be added twice, and it's the one added by 5->4 
					// that counts. 
					if(explored[v])
						continue;
					explored[v] = true;
					m_SCCs.back().push_back(v);
					m_SCCSizes.back()++;
					const vector<int>& outEdges = m_G.getOutEdgesOf(v);
					for(auto it = outEdges.begin(); it != outEdges.end(); ++it) {
						if(!explored[*it]) {
							vertices.push(*it);
						}
					}
				}
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