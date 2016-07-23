#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <cassert>
using std::map;
using std::ifstream;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using std::vector;
bool DEBUG = false;
int comp(int i, int j) {
	if(i < j) return -1;
	if(i > j) return 1;
	return 0;
}
struct Edge {
	Edge() = default;
	Edge(int n1, int n2, int l) : node1(n1), node2(n2), length(l) {}
	int node1;
	int node2;
	int length;
	int comp(const Edge& e) const { return ::comp(length, e.length); }
};
int comp(const Edge& e1, const Edge& e2) { return e1.comp(e2); }
class Graph {
public:
	Graph(istream& is) {
		is >> _numNodes;
		int i, j, d;
		while(is >> i >> j >> d) {
			_edges.emplace_back(i, j, d);
		}
	}
	size_t numNodes() const { return _numNodes; }
	size_t numEdges() const { return _edges.size(); }
	vector<Edge>& getEdges() { return _edges; }
	const vector<Edge>& getEdges() const { return _edges; }
private:
	size_t _numNodes;
	vector<Edge> _edges;
};
ostream& operator<<(ostream& os, const Graph& g) {
	os << g.numNodes() << " " << g.numEdges() << "\n";
	for(auto it = g.getEdges().begin(); it != g.getEdges().end(); it++) {
		os << it->node1 << " " << it->node2 << " " << it->length << "\n";
	}
	return os;
}
class UnionFind {
public:
	UnionFind(int n) : _leader(n + 1), _size(n + 1, 1), _count(n) {
		for(int i = 1; i <= n; i++){
			_leader[i] = i;
		}
	}
	void unite(int i, int j) {
		int id1 = find(i);
		int id2 = find(j);
		if(id1 != id2) {
			if(_size[id1] < _size[id2]) {
				_leader[id1] = id2;
				_size[id2] += _size[id1]; 
			} else {
				_leader[id2] = id1;
				_size[id1] += _size[id2];
			}
			_count--;
		}
	}
	int find(int i) const {
		while(i != _leader[i]) {
			i = _leader[i];
		}
		return i;
	}
	bool connected(int i, int j) const { return find(i) == find(j); }
	int count() const { return _count; }
private:
	vector<int> _leader;
	vector<int> _size;
	int _count;
};
class Stack {
public:
	Stack() = default;
	void push(int i) { _vals.push_back(i); }
	int pop() {
		int t = _vals.back();
		_vals.resize(_vals.size() - 1);
		return t;
	}
	size_t size() const { return _vals.size(); }
	bool empty() const { return size() == 0; }
private:
	vector<int> _vals;
};
template <typename T>
class QuickSort {
public:
	static void sort(vector<T>& v, int (*comp)(const T& t1, const T& t2)) {
		Stack lefts, rights;
		lefts.push(0);
		rights.push(v.size() - 1);
		while(!lefts.empty()) {
			int left = lefts.pop();
			int right = rights.pop();
			int pivot = left + rand() % (right - left + 1);
			int pos = partition(v, left, right, pivot);
			if(left < pos - 1) {
				lefts.push(left);
				rights.push(pos - 1);
			}
			if(pos + 1 < right) {
				lefts.push(pos + 1);
				rights.push(right);
			}
		}
	}
private:
	static void swap(vector<T>& v, int i, int j) {
		if(i != j) {
			T temp = v[i];
			v[i] = v[j];
			v[j] = temp;
		} 
	}
	static int partition(vector<T>& v, int left, int right, int pivot) {
		swap(v, left, pivot);
		int firstLarger = left + 1;
		for(int i = left + 1; i <= right; ++i) {
			if(comp(v[i], v[left]) < 0) {
				swap(v, i, firstLarger++);
			}
		}
		swap(v, firstLarger - 1, left);
		return firstLarger - 1;
	}
};
class Clustering {
public:
	Clustering(const Graph& g) : _g(g), _uf(g.numNodes()) {
		QuickSort<Edge>::sort(_g.getEdges(), comp);
		assert(_g.getEdges().size() != 0);
		if(DEBUG)
			cout << _g << endl;
		_spacing = _g.getEdges()[0].length;
	}
	void computeCluster(int k) {
		if(DEBUG) {
			for(int i = 1; i <= _g.numNodes(); ++i)
				cout << i << "->" << _uf.find(i) << "\n";
			cout << endl;
		}
		auto it = _g.getEdges().begin();
		while(numClusters() > k && it != _g.getEdges().end()){
			_uf.unite(it->node1, it->node2);
			++it;
		}
		while(it != _g.getEdges().end() && _uf.connected(it->node1, it->node2)) 
			++it;
		_spacing = (it == _g.getEdges().end()) ? 0 : it->length;

		_clusters = map<int, vector<int> >();
		for(int i = 1; i <= _g.numNodes(); ++i) {
			if(DEBUG)
				cout << i << "->" << _uf.find(i) << endl;
			_clusters[_uf.find(i)].push_back(i);
		}
	}
	int spacing() const { return _spacing; }
	size_t numClusters() const { return _uf.count(); }
	const map<int, vector<int> >& clusters() const { return _clusters; }
private:
	Graph _g;
	map<int, vector<int> > _clusters;
	UnionFind _uf;
	int _spacing;
};
int main (int argc, char** argv) {
	if(argc > 3) 
		DEBUG = true;
	ifstream ifs(argv[1]);
	Graph g(ifs);
	Clustering clustering(g);
	clustering.computeCluster(atoi(argv[2]));
	cout << "spacing " << clustering.spacing() << endl;
	cout << "number of clusters: " << clustering.numClusters() << endl;
	auto& clusters = clustering.clusters();
	for(auto it = clusters.begin(); it != clusters.end(); it++) {
		for(auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			cout << *itt << " ";
		}
		cout << "\n";
	}
	cout << std::flush;
	return 0;
}