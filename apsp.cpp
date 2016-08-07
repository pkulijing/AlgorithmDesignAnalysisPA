#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cassert>
#include <string>
#include <exception>
#include <climits>
#include <list>
#include <string>
#include <cstdlib>
#include <ctime>
#include <future>
using std::string;
using std::list;
using std::cout;
using std::vector;
using std::ifstream;
using std::istream;
using std::ostream;
using std::string;
using std::map;
using std::endl;
using std::future;
using std::async;
//Only used for invalid value of array index. For length, use INT_MAX.
const int INVALID = -1;
struct Edge{
	Edge() = default;
	Edge(int t, int h, int l) : tail(t), head(h), length(l) {}
	int tail;
	int head;
	int length;
};
class Graph {
public:
	Graph(istream& is) {
		int numEdges;
		is >> _numNodes >> numEdges;
		_edges.reserve(numEdges);
		_outEdges.resize(_numNodes + 1);
		_inEdges.resize(_numNodes + 1);
		for(int i = 0; i < numEdges; i++) {
			int t, h, l;
			is >> t >> h >> l;
			_edges.emplace_back(t, h, l);
			_outEdges[t].emplace_back(t, h, l);
			_inEdges[h].emplace_back(t, h, l);
		}
	}
	Graph(int n) : _numNodes(n), _outEdges(n + 1), _inEdges(n + 1) {}
	void addNode() {
		_numNodes++;
		_outEdges.resize(_outEdges.size() + 1);
		_inEdges.resize(_inEdges.size() + 1);
	}
	void addEdge(int t, int h, int l) {
		_edges.emplace_back(t, h, l);
		_outEdges[t].emplace_back(t, h, l);
		_inEdges[h].emplace_back(t, h, l);
	}
	void addEdge(const Edge& e) { addEdge(e.tail, e.head, e.length); }
	int numNodes() const { return _numNodes; }
	int numEdges() const { return _edges.size(); }
	const vector<Edge>& edgesFrom(int i) const { return _outEdges[i]; }
	const vector<Edge>& edgesTo(int i) const { return _inEdges[i]; }
	const vector<Edge>& edges() const { return _edges; }
protected:
	int _numNodes;
	vector<Edge> _edges;
	vector<vector<Edge> > _outEdges;
	vector<vector<Edge> > _inEdges;
};
ostream& operator<<(ostream& os, const Graph& g) {
	os << g.numNodes() << " " << g.numEdges() << "\n";
	for(auto it = g.edges().begin(); it != g.edges().end(); ++it) {
		os << it->tail << " " << it->head << " " << it->length << "\n";
	} 
	os << std::flush;
	return os;
};
class Heap {
public:
	struct Node {
		Node() = default;
		Node(int i, int k) : index(i), key(k) {}
		int index;
		int key;
	};
	Heap(size_t cap) : _indexToPos(cap + 1) {
		//do not use position 0
		_nodes.emplace_back(INVALID,INVALID);
	}
	void add(const Node& n) {
		assert(size() < capacity());
		_nodes.push_back(n);
		_indexToPos[n.index] = size();
		bubble(size());
	}
	Node getMin() {
		assert(size() > 0);
		Node n = _nodes[1];
		_indexToPos[n.index] = INVALID;
		_nodes[1] = _nodes.back();
		_indexToPos[_nodes[1].index] = 1;
		_nodes.resize(_nodes.size() - 1);
		sink(1);
		return n;
	}
	int getKey(int index) const { 
		assert(contains(index));
		return _nodes[_indexToPos[index]].key;
	}
	void changeKey(int index, int newKey) {
		int oldKey = getKey(index);
		_nodes[_indexToPos[index]].key = newKey;
		if(newKey > oldKey) {
			sink(_indexToPos[index]);
		} else if(newKey < oldKey) {
			bubble(_indexToPos[index]);
		}
	}
	size_t capacity() const { return _indexToPos.size() - 1; }
	size_t size() const { return _nodes.size() - 1; }
	bool empty() const { return size() == 0; }
	bool contains(int index) const { return _indexToPos[index] != INVALID; }
	void output(ostream& os) const {
		for(auto it = _nodes.begin(); it != _nodes.end(); ++it) {
			os << "(" << it->index << "," << it->key << ")" << endl;
		}
	}
private:
	void bubble(int pos) {
		while(hasParent(pos) && less(pos, pos / 2)) {
			swap(pos, pos / 2);
			pos /= 2;
		}
	}
	void sink(int pos) {
		while(hasLeftChild(pos)) {
			int child = pos * 2;
			if(hasRightChild(pos) && less(child + 1, child)) {
				child++;
			}
			if(!less(child, pos))
				return;
			swap(pos, child);
			pos = child;
		}
	}
	void swap(int p1, int p2) {
		if(p1 != p2) {
			_indexToPos[_nodes[p1].index] = p2;
			_indexToPos[_nodes[p2].index] = p1;
			Node n = _nodes[p1];
			_nodes[p1] = _nodes[p2];
			_nodes[p2] = n;
		}
	}
	bool hasParent(int pos) const { return pos > 1; } 
	bool hasLeftChild(int pos) const { return 2 * pos <= size(); }
	bool hasRightChild(int pos) const { return 2 * pos + 1 <= size(); }
	bool less(int p1, int p2) const { return _nodes[p1].key < _nodes[p2].key; }
	vector<Node> _nodes;
	vector<int> _indexToPos;
};
class Dijkstra {
public:
	Dijkstra(Graph& g, int s) : _source(s), _heap(g.numNodes()), 
		_lengths(g.numNodes() + 1), _prev(g.numNodes() + 1, INVALID) {
		for(int i = 1; i <= g.numNodes(); i++) {
			int len = (i == s ? 0 : INT_MAX);
			_heap.add(Heap::Node(i, len));
		}
		_prev[_source] = INVALID;
		while(!_heap.empty()) {
			Heap::Node n = _heap.getMin();
			_lengths[n.index] = n.key;
			const vector<Edge>& edges = g.edgesFrom(n.index);
			for(auto it = edges.begin(); it != edges.end(); ++it) {
				if(_heap.contains(it->head) && n.key != INT_MAX && n.key + it->length < _heap.getKey(it->head)) {
					_heap.changeKey(it->head, n.key + it->length);
					_prev[it->head] = it->tail;
				}
			}
		}
	}
	int lengthTo(int index) const { return _lengths[index]; }
	list<int> pathTo(int index) const {
		list<int> res;
		if(_prev[index] != INVALID || index == _source) {
			while(index != _source) {
				res.push_front(index);
				index = _prev[index];
			}
			res.push_front(_source);
		}
		return res;
	}
private:
	const int _source;
	Heap _heap;
	vector<int> _lengths;
	vector<int> _prev;
};
class BellmanFord {
public:
	BellmanFord(const Graph& g, int s) :
		_source(s),
		_numNodes(g.numNodes()),
	 	_lengths(_numNodes + 1, INT_MAX),
	 	_prev(g.numNodes() + 1, INVALID),
	 	_negativeCycle(false) {
	 	_lengths[_source] = 0;
	 	vector<future<void> > futures;
 		for(int i = 1; i < _numNodes; i++) {
 			futures.push_back(async(
 				[&]() mutable {
					for(int j = 1; j <= _numNodes; j++) {
	 					const vector<Edge>& edges = g.edgesTo(j);
		 				for(auto it = edges.begin(); it != edges.end(); ++it) {
		 					if(_lengths[it->tail] != INT_MAX && _lengths[it->tail] + it->length < _lengths[j]) {
		 						_lengths[j] = _lengths[it->tail] + it->length;
		 						_prev[j] = it->tail;
		 					}
						}
					}
				}));
 		}
 		for(auto& f : futures) {
 			f.get();
 		}

 		for(int j = 1; j <= _numNodes; j++) {
			const vector<Edge>& edges = g.edgesTo(j);
			for(auto it = edges.begin(); it != edges.end(); ++it) {
				if(_lengths[it->tail] != INT_MAX && _lengths[it->tail] + it->length < _lengths[j]) {
					_lengths[j] = _lengths[it->tail] + it->length;
 					_prev[j] = it->tail;
					_negativeCycle = true;
				}
			}
		}
	}
	bool hasNegativeCycle() const { return _negativeCycle; }
	int lengthTo(int index) const { return _lengths[index]; }
	list<int> pathTo(int index) const {
		list<int> res;
		if(_prev[index] != INVALID || index == _source) {
			while(index != _source) {
				res.push_front(index);
				index = _prev[index];
			}
			res.push_front(_source);
		}
		return res;
	}
private:
	const int _source;
	const int _numNodes;
	bool _negativeCycle;
	vector<int> _lengths;
	vector<int> _prev;
};
class FloydWarshall {
public:
	FloydWarshall(Graph& g) : _numNodes(g.numNodes()),
		_lengths(_numNodes + 1, 
		vector<vector<int> >(_numNodes + 1, vector<int>(2, INT_MAX))) {
		for(int i = 1; i <= _numNodes; i++) {
			_lengths[i][i][0] = 0;
		}
		const vector<Edge>& edges = g.edges();
		for(auto it = edges.begin(); it != edges.end(); ++it) {
			_lengths[it->tail][it->head][0] = it->length;
		}
		for(int k = 1; k <= _numNodes; k++) {
			for(int i = 1; i <= _numNodes; i++) {
				for(int j = 1; j <= _numNodes; j++) {
					_lengths[i][j][k % 2] = _lengths[i][j][1 - k % 2];
					if(_lengths[i][k][1 - k % 2] != INT_MAX && _lengths[k][j][1 - k % 2] != INT_MAX && 
						_lengths[i][k][1 - k % 2] + _lengths[k][j][1 - k % 2] < _lengths[i][j][k % 2]) {
						_lengths[i][j][k % 2] = _lengths[i][k][1 - k % 2] + _lengths[k][j][1 - k % 2];
					}
				}
			}
		}

	}
	bool hasNegativeCycle() const {
		for(int i = 1; i <= _numNodes; i++) {
			if(length(i, i) < 0)
				return true;
		}
		return false;
	}
	int length(int i, int j) const { return _lengths[i][j][_numNodes % 2]; }
	int shortest() const {
		int res = INT_MAX;
		for(int i = 1; i <= _numNodes; i++) {
			for(int j = 1; j <= _numNodes; j++) {
				res = res < length(i, j) ? res : length(i, j);
			}
		}
		return res;
	}
private:
	const int _numNodes;
	vector<vector<vector<int> > > _lengths;
};
class Johnson {
public:
	Johnson(const Graph& g) : 
		_negativeCycle(false), 
		_numNodes(g.numNodes()), 
		_lengths(_numNodes + 1, vector<int>(_numNodes + 1, INT_MAX)) {
		Graph g0 = g;
		g0.addNode();
		//Do not include the new node!
		for(int i = 1; i <= _numNodes; i++) {
			g0.addEdge(_numNodes + 1, i, 0);
		}
		BellmanFord bf(g0, _numNodes + 1);
		if(bf.hasNegativeCycle()) {
			_negativeCycle = true;
		} else {
			cout << "Does not have negative cycle. Will continue to run ..." << endl;
			Graph g1(_numNodes);
			for(auto it = g.edges().begin(); it != g.edges().end(); ++it) {
				g1.addEdge(it->tail, it->head, it->length + bf.lengthTo(it->tail) - bf.lengthTo(it->head));
			}
			vector<future<void> > futures;
			for(int i = 1; i <= _numNodes; i++) {
				if(i % 100 == 0) {
					cout << "patching job of node " << i << endl;
				}
				futures.push_back(async(
					[&](int index) mutable {
						Dijkstra ds(g1, index);
						for(int j = 1; j <= _numNodes; j++) {
							int len = ds.lengthTo(j);
							if(len != INT_MAX) {
								len = len + bf.lengthTo(j) - bf.lengthTo(i);
							} 
							_lengths[index][j] = len;
						}
					}, i));
			}
			for(auto& f : futures) {
				f.get();
			}
		}
	}
	bool hasNegativeCycle() const { return _negativeCycle; }
	int length(int i, int j) const { return _lengths[i][j]; }
	int shortest() const {
		int res = INT_MAX;
		for(int i = 1; i <= _numNodes; i++) {
			for(int j = 1; j <= _numNodes; j++) {
				res = res < length(i, j) ? res : length(i, j);
			}
		}
		return res;
	}
private:
	int _numNodes;
	vector<vector<int> > _lengths;
	bool _negativeCycle;
};
int main_test(int argc, char** argv) {
	for(int i = 2; i < argc; i++) {
		ifstream ifs(argv[i]);
		Graph g(ifs);
		cout << g << "Dijkstra : \n";
		Dijkstra dijkstra(g, 1);
		for(int i = 1; i <= g.numNodes(); i++) {
			cout << i << ":" << dijkstra.lengthTo(i) << endl;
		}
		for(int i = 1; i <= g.numNodes(); i++) {
			list<int> path = dijkstra.pathTo(i);
			cout << i << ": ";
			if(path.size() != 0) {
				cout << "[";
				for(auto it = path.begin(); it != path.end(); ++it) {
					cout << *it << "->";
				}
			}
			cout << endl;
		}	
		cout << "Bellman-Ford : \n";
		BellmanFord bellmanford(g, 1);
		cout << "hasNegativeCycle: " << bellmanford.hasNegativeCycle() << endl;
		for(int i = 1; i <= g.numNodes(); i++) {
			cout << i << ":" << bellmanford.lengthTo(i) << endl;
		}
		for(int i = 1; i <= g.numNodes(); i++) {
			list<int> path = bellmanford.pathTo(i);
			cout << i << ": ";
			if(path.size() != 0) {
				cout << "[";
				for(auto it = path.begin(); it != path.end(); ++it) {
					cout << *it << "->";
				}
			}
			cout << endl;
		}
		cout << "Floyd-Warshall:\n";
		FloydWarshall floydwarshall(g);
		for(int i = 1; i <= g.numNodes(); i++) {
			cout << i << ":" << floydwarshall.length(1, i) << endl;
		}
		cout << "hasNegativeCycle: " << floydwarshall.hasNegativeCycle() << endl;
		cout << "shortest of shortest: " << floydwarshall.shortest() << endl;
		cout << "Johnson:\n";
		Johnson johnson(g);
		for(int i = 1; i <= g.numNodes(); i++) {
			cout << i << ":" << johnson.length(1, i) << endl;
		}
		cout << "hasNegativeCycle: " << johnson.hasNegativeCycle() << endl;
		if(!johnson.hasNegativeCycle()) {
			cout << "shortest of shortest: " << johnson.shortest() << endl;
		}
	}
	return 0;
}
int main_fw(int argc, char** argv) {
	for(int i = 2; i < argc; i++) {
		ifstream ifs(argv[i]);
		time_t t;
		time(&t);
		cout << "Begin with file " << argv[i] << " at " << asctime(localtime(&t));
		cout << "constructing graph ...\n";
		Graph g(ifs);
		cout << "Running Floyd Warshall algorithm ...\n";
		FloydWarshall floydwarshall(g);
		cout << "hasNegativeCycle: " << floydwarshall.hasNegativeCycle() << endl;
		cout << "shortest of shortest: " << floydwarshall.shortest() << endl;
		time(&t);
		cout << "End with file " << argv[i] << " at " << asctime(localtime(&t)) << endl;
	}
	return 0;
}
int main_johnson(int argc, char** argv) {
	for(int i = 2; i < argc; i++) {
		ifstream ifs(argv[i]);
		time_t t;
		time(&t);
		cout << "Begin with file " << argv[i] << " at " << asctime(localtime(&t));
		cout << "constructing graph ...\n";
		Graph g(ifs);
		cout << "Running Johnson algorithm ...\n";
		Johnson johnson(g);
		cout << "hasNegativeCycle: " << johnson.hasNegativeCycle() << endl;
		if(!johnson.hasNegativeCycle()) {
			cout << "shortest of shortest: " << johnson.shortest() << endl;
		}
		time(&t);
		cout << "End with file " << argv[i] << " at " << asctime(localtime(&t)) << endl;
	}
	return 0;
}
int main(int argc, char** argv) {
	if(atoi(argv[1]) == 0) {
		return main_test(argc, argv);
	} else if(atoi(argv[1]) == 1) {
		return main_fw(argc, argv);
	} else {
		return main_johnson(argc, argv);
	}
}