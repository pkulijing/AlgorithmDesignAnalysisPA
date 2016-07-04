#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <cassert>
using std::cout;
using std::vector;
using std::ifstream;
using std::istream;
using std::ostream;
using std::istringstream;
class Heap;
class Dijkstra;
class Graph;
class Graph {
public:
	Graph(istream& is) {

	}
private:
	vector<vector<int> > m_Edges;
	vector<vector<int> > m_EdgeLengths;
};
class Dijkstra {
public:
	Dijkstra(const Graph& G, int s = 1) : m_G(G), m_Source(s) {

	}

private:
	const Graph& m_G;
	const int m_Source;
};
template <typename T>
class Heap {
public:
	struct Node {
		T index;
		int key;
	};
	Heap(const vector<Node>& v) {

	}
	void add(const Node& node) {
		assert(m_LookUpTable.find(node.index) == m_LookUpTable.end());
		m_Nodes.push_back(node);
		m_LookUpTable[node.index] = m_Nodes.size() - 1;
		bubble_inner(m_Nodes.size() - 1);
	}
	Node getMin() {
		Node n = m_Nodes[1];
		m_Nodes[1] = m_Nodes.back();
		sink_inner(1);
		m_Nodes.resize(m_Nodes.size() - 1);
		return n;
	}
	void sink(const T& index) {
		auto it = m_LookUpTable.find(index);
		assert(it != m_LookUpTable.end());
		sink_inner(it->second);
	}
	void bubble(const T& index) {
		auto it = m_LookUpTable.find(index);
		assert(it != m_LookUpTable.end());
		bubble_inner(it->second);
	}
	int size() const { return m_Nodes.size() - 1; }
private:
	void sink_inner(int i) {
		while(has_left_son(i)) {
			int j = 2 * i;
			//j should be the smaller one of the two sons
			if(has_right_son() && less(j + 1, j))
				j++;
			if(!less(j, i))
				break;

			swap(i, j);
			i = j;
		}

		 && less(2 * i, i)) {
			swap(i, 2 * i);
		}
	}
	void bubble_inner(int i) {
		while(has_parent(i) && less(i, i / 2)) {
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
	bool has_left_son(int i) {
		return 2 * i <= size();
	}
	bool has_right_son(int i) {
		return 2 * i + 1 <= size();
	}
	bool has_parent(int i) {
		return i > 1;
	}
	bool less(int i, int j) {
		m_Nodes[i].key < m_Nodes[j].key;
	}
	vector<Node> m_Nodes;
	map<const T&,i> m_LookUpTable;
};
