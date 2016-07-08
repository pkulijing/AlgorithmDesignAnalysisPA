#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cassert>
using std::ostream;
using std::istream;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::vector;
using std::map;
const bool MAX = true;
const bool MIN = false;
template <typename T>
class Heap {
public:
	struct Node {
		Node() = default;
		Node(const T& i, int k) : index(i), key(k) {}
		T index;
		int key;
	};
	Heap(bool minmax) : m_MinOrMax(minmax) {
		//We don't use index 0
		m_Nodes.push_back(Node(-1,-1));
	}
	void insert(const Node& node) {
		assert(m_LookUpTable.find(node.index) == m_LookUpTable.end());
		m_Nodes.push_back(node);
		m_LookUpTable[node.index] = m_Nodes.size() - 1;
		bubble(m_Nodes.size() - 1);
	}
	Node extractRoot() {
		Node n = m_Nodes[1];
		m_LookUpTable.erase(n.index);
		m_Nodes[1] = m_Nodes.back();
		m_Nodes.resize(m_Nodes.size() - 1);
		sink(1);
		return n;
	}
	Node getRoot() const {
		return m_Nodes[1];
	}
	int getKey(const T& index) const {
		auto it = m_LookUpTable.find(index);
		assert(it != m_LookUpTable.end());
		return m_Nodes[it->second].key;
	}
	void changeKey(const T& index, int key) {
		int i = m_LookUpTable[index];
		int oldKey = m_Nodes[i].key;
		m_Nodes[i].key = key;
		if(key < oldKey)
			bubble(i);
		else if(key > oldKey)
			sink(i);
	}
	int size() const { return m_Nodes.size() - 1; }
	bool empty() const { return size() == 0; }
	bool contains(const T& index) const { return m_LookUpTable.find(index) != m_LookUpTable.end(); }
private:
	void sink(int i) {
		while(hasLeftSon(i)) {
			int j = 2 * i;
			//j should be the smaller one of the two sons for MinHeap, and the larger for max heap
			if(hasRightSon(i) && compare(j + 1, j))
				j++;
			if(!compare(j, i))
				break;
			swap(i, j);
			i = j;
		}
	}
	void bubble(int i) {
		while(hasParent(i) && compare(i, i / 2)) {
			swap(i, i / 2);
			i /= 2;
		}
	}
	void swap(int i, int j) {
		m_LookUpTable[m_Nodes[i].index] = j;
		m_LookUpTable[m_Nodes[j].index] = i;
		Node n = m_Nodes[i];
		m_Nodes[i] = m_Nodes[j];
		m_Nodes[j] = n;
	}
	bool hasLeftSon(int i) {
		return 2 * i <= size();
	}
	bool hasRightSon(int i) {
		return 2 * i + 1 <= size();
	}
	bool hasParent(int i) {
		return i > 1;
	}
	bool compare(int i, int j) {
		return m_MinOrMax == MIN ? m_Nodes[i].key < m_Nodes[j].key : m_Nodes[i].key > m_Nodes[j].key;
	}
	vector<Node> m_Nodes;
	map<T,int> m_LookUpTable;
	const bool m_MinOrMax;
};
class MedianMaintenance {
public:
	MedianMaintenance() : m_MaxHeap(Heap<int>(MAX)), m_MinHeap(Heap<int>(MIN)) {}
	void add(int num) {
		if(m_MaxHeap.empty() || num < m_MaxHeap.getRoot().key)
			m_MaxHeap.insert(Heap<int>::Node(num,num));
		else 
			m_MinHeap.insert(Heap<int>::Node(num,num));
		if(m_MinHeap.size() - m_MaxHeap.size() == 2) {
			m_MaxHeap.insert(m_MinHeap.extractRoot());
		} else if(m_MaxHeap.size() - m_MinHeap.size() == 2) {
			m_MinHeap.insert(m_MaxHeap.extractRoot());
		}
		if(m_MinHeap.size() > m_MaxHeap.size()) {
			m_Medians.push_back(m_MinHeap.getRoot().key);
		} else {
			m_Medians.push_back(m_MaxHeap.getRoot().key);
		}
	}
	int size() const { return m_Medians.size(); }
	const vector<int>& getMedians() const { return m_Medians; }
private:
	vector<int> m_Medians;
	Heap<int> m_MaxHeap, m_MinHeap; //maxheap: smaller half. minheap:larger half
};
int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	MedianMaintenance mm;
	int num;
	while(ifs >> num) {
		mm.add(num);
	}
	const vector<int>& medians = mm.getMedians();
	cout << medians.size() << endl;
	// for(auto it = medians.begin(); it != medians.end(); ++it) {
	// 	cout << *it << ",";
	// }
	cout << std::accumulate(medians.begin(), medians.end(), 0) << endl;
	return 0;
}

