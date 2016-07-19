#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <cassert>
#include <climits>
using std::map;
using std::pair;
using std::make_pair;
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;
using std::istream;
using std::ostream;
struct Edge{
	Edge() = default;
	Edge(int n1, int n2, int c) : node1(n1), node2(n2), cost(c) {}
	int node1;
	int node2;
	int cost;
};
class Graph {
public:
	Graph(istream& is){
		int numNodes, numEdges;
		is >> numNodes >> numEdges;
		_numEdges = numEdges;
		_adjacents.resize(numNodes + 1);
		for(int i = 0; i < numEdges; i++) {
			int n1, n2, c;
			is >> n1 >> n2 >> c;
			assert(n1 != n2);
			_adjacents[n1].emplace_back(n1, n2, c);
			_adjacents[n2].emplace_back(n1, n2, c);
		}
	}
	int numNodes() const { return _adjacents.size() - 1; }
	int numEdges() const { return _numEdges; }
	const vector<Edge>& getAdjacents(int i) const { return _adjacents[i]; }
private:
	int _numEdges;
	vector<vector<Edge> > _adjacents;
};
ostream& operator<<(ostream& os, const Graph& g) {
	os << g.numNodes() << " nodes, " << g.numEdges() << " edges.\n";
	for(int i = 1; i <= g.numNodes(); i++) {
		os << i << ": ";
		auto& adjs = g.getAdjacents(i);
		for(auto it = adjs.begin(); it != adjs.end(); ++it) {
			os << "(" << it->node1 << "--" << it->node2 << ", " << it->cost << ")";
		}
		os << "\n";
	}
	return os;
}
class Heap {
public:
	struct Node {
		Node() = default;
		Node(int i, int k) : index(i), key(k) {}
		int index;
		int key;
	};
	Heap() : _nodes(vector<Node>(1)) {}
	void insert(int index, int key) {
		assert(_lookUpTable.find(index) == _lookUpTable.end());
		_nodes.emplace_back(index, key);
		_lookUpTable[index] = size();
		swim(size());
	}
	void insert(const Node& n) { insert(n.index, n.key); }
	Node extractMin() {
		Node n = _nodes[1];
		_lookUpTable.erase(n.index);
		swap(1, size());
		_nodes.resize(_nodes.size() - 1);
		sink(1);
		return n;
	}
	int getKey(int index) {
		assert(_lookUpTable.find(index) != _lookUpTable.end());
		return _nodes[_lookUpTable[index]].key;	
	}
	void changeKey(int index, int key) {
		int i = _lookUpTable[index];
		int oldKey = _nodes[i].key;
		_nodes[i].key = key;
		if(key > oldKey)
			sink(i);
		else if(key < oldKey) {
			swim(i);
		}
	}
	size_t size() const { return _nodes.size() - 1;}
	bool empty() const { return size() == 0; }
private:
	bool less(int i, int j) { return _nodes[i].key < _nodes[j].key; }
	void sink(int i) {
		while(hasLeftChild(i)) {
			int j = 2 * i;
			if(hasRightChild(i) && less(j + 1, j))
				j++;
			if(!less(j, i))
				break;
			swap(i, j); 
			i = j;
		}
	}
	void swim(int i) {
		while(hasParent(i) && less(i, i / 2) ) {
			swap(i, i / 2);
			i = i / 2;
		}
	}
	int hasParent(int i) const { return i > 1; }
	int hasLeftChild(int i) const { return 2 * i <= size(); }
	int hasRightChild(int i) const { return 2 * i + 1 <= size(); }
	void swap(int i, int j) {
		if(i == j) return;
		_lookUpTable[_nodes[i].index] = j;
		_lookUpTable[_nodes[j].index] = i;

		Node temp = _nodes[i];
		_nodes[i] = _nodes[j];
		_nodes[j] = temp;
	}
	vector<Node> _nodes; //array index -> node in graph
	map<int,int> _lookUpTable; // node in graph -> array index
};
class PrimMST {
public:
	PrimMST(const Graph& g) : _g(g), _costMST(0), _inMST(vector<bool>(g.numNodes())) {
		Heap minHeap;
		for(int i = 1; i <= g.numNodes(); i++) {
			minHeap.insert(i, INT_MAX);
		}
		while(!minHeap.empty()) {
			Heap::Node min = minHeap.extractMin();
			if(min.key != INT_MAX)
				_costMST += min.key;
			_inMST[min.index] = true;
			const vector<Edge>& adjs = g.getAdjacents(min.index);
			for(auto it = adjs.begin(); it != adjs.end(); ++it) {
				int other = (it->node1 == min.index) ? it->node2 : it->node1;
				if(!_inMST[other] && it->cost < minHeap.getKey(other))
					minHeap.changeKey(other, it->cost);
			}
		}
	}
	int64_t cost() const { return _costMST; }
private:
	const Graph& _g;
	int64_t _costMST;
	vector<bool> _inMST;
};

int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	Graph g(ifs);
	cout << g << endl;
	cout << PrimMST(g).cost() << endl;
	return 0;
}