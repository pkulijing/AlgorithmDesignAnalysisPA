#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <cstdlib>
#include <utility>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
using namespace std;
class Graph;
class MinCut;
struct Cut;
class UnionFind;
class UnionFind {
public:
	UnionFind(int n) : count(n), parent(vector<int>(n)) {
		for(int i = 0; i < n; i++) {
			parent[i] = i;
		}
	}
	bool connected(int i, int j) {
		return find(i) == find(j);
	}
	void unite(int i, int j) {
		int rooti = find(i);
		int rootj = find(j);
		if(rootj == rooti) return;
		parent[rooti] = parent[rootj];
	}
	int find(int i) {
		while(i != parent[i])
			i = parent[i];
		return i;
	}
private:
	vector<int> parent;
	int count;
};
class Graph {
public:
	Graph(istream& is) : m_NumEdges(0) {
		string line;
		while(getline(is, line)) {
			if(line.length() == 0)
				continue;
			istringstream iss(line);
			list<int> adjs;
			int tmp;
			iss >> tmp;
			m_Vertices.push_back(tmp);
			while(iss >> tmp) {
				adjs.push_back(tmp);
			}
			m_Adjs.push_back(adjs);
			m_NumEdges += adjs.size();
		}
	}
	int numVertices() const { return m_Vertices.size(); }
	int numEdges() const { return m_NumEdges; }
	list<list<int> >::iterator adjs(int i) { 
		auto it = m_Adjs.begin();
		int j = 0;
		for(auto itv = m_Vertices.begin(); itv != m_Vertices.end(); ++itv) {
			if(*itv != i)
				++j;
			else break;
		}
		advance(it, j);
		return it;
	}	
	list<list<int> >::const_iterator adjs(int i) const{ 
		auto it = m_Adjs.begin();
		int j = 0;
		for(auto itv = m_Vertices.begin(); itv != m_Vertices.end(); ++itv) {
			if(*itv != i)
				++j;
			else break;
		}
		advance(it, j);
		return it;
	}
	void output(ostream& os) const {
		os << "number of vertices: " << numVertices() << endl;
		os << "number of edges: " << numEdges() << endl;
		os << "Edges: " << endl;
		auto it1 = m_Vertices.begin();
		auto it2 = m_Adjs.begin();
		while(it1 != m_Vertices.end()) {
			os << *it1 << " ";
			for(auto it = it2->begin(); it != it2->end(); ++it) {
				os << *it << " ";
			}
			os << endl;
			++it1;
			++it2;
		}
	}
	pair<int,int> randomEdge() const {
		int iEdge = rand() % numEdges();
		for(auto it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
			int i = *it;
			if(iEdge >= adjs(i)->size()) {
				iEdge -= adjs(i)->size();
				continue;
			}
			auto iti = adjs(i)->begin();
			advance(iti, iEdge);
			return make_pair(i, *iti);
		}
		//cannot reach here
		return make_pair(-1,-1);
	}
	void merge(int i, int j) {
		auto adjsi = adjs(i);
		auto adjsj = adjs(j);
		int numSelfLoop = 0;
		//remove self loops on j's side
		auto itj = adjsj->begin();
		while(itj != adjsj->end()) {
			if(*itj == i) {
				itj = adjsj->erase(itj);
				numSelfLoop++;
			}
			else
				++itj;
		}
		//for each of i's adjacencies, check if it's j
		for(auto iti = adjsi->begin(); iti != adjsi->end(); ++iti) {
			if(*iti == j) {
				//If so, self loop.
				numSelfLoop++;
				continue;
			}
			//otherwise, change i to j in the adjacencies of this vertex, and add it to adjacencies of j.
			auto adjsk = adjs(*iti);
			for(auto itk = adjsk->begin(); itk != adjsk->end(); ++itk) {
				if(*itk == i) {
					*itk = j;
					adjsj->push_back(*iti);
				}
			}
		}
		auto itv = m_Vertices.begin();
		while(*itv != i)
			itv++;
		m_Vertices.erase(itv);
		m_NumEdges -= numSelfLoop;
		m_Adjs.erase(adjsi);
	}
private:
	int m_NumEdges;
	list<list<int> > m_Adjs;
	list<int> m_Vertices;
};
ostream& operator<<(ostream& os, const Graph& G) {
	G.output(os);
	return os;
}
struct Cut {
	int numCrossingEdges;
	vector<int> left;
	vector<int> right;
	vector<pair<int, int> > edges;
};
ostream& operator<<(ostream& os, const Cut& cut) {
	os << "number of crossing edges: " << cut.numCrossingEdges << endl;
	os << "first part: " << endl;
	for(auto it = cut.left.begin(); it != cut.left.end(); ++it)
		os << *it << " ";
	os << endl;
	os << "second part: " << endl;
	for(auto it = cut.right.begin(); it != cut.right.end(); ++it)
		os << *it << " ";
	os << endl;
	os << "Crossing edges: " << endl;
	for(auto it = cut.edges.begin(); it != cut.edges.end(); ++it) {
		os << "(" << it->first << ", " << it->second << ")";
	}	
	os << endl;
	return os;
}
class MinCut {
public:
	MinCut(const Graph& G):m_Graph(G) {}
	Cut computeMinCut(int numIt) {
		cout << "Start ..." << endl;
		Cut best;
		best.numCrossingEdges = m_Graph.numEdges();
		int success = 1;
		for(int i = 0; i < numIt; ++i) {
			Cut res = randomContract();
			if(res.numCrossingEdges < best.numCrossingEdges) {
				best = res;
				success = 1;
			} else if(res.numCrossingEdges == best.numCrossingEdges) {
				success++;
			}
			cout << "Iterator = " << i << ", res = " << res.numCrossingEdges << ", best = " << best.numCrossingEdges << endl;
		}
		cout << "Best case appeared " << success << "times, success rate = " << (double)success / numIt << endl;
		return best;
	} 
private:
	Cut randomContract() const {
		Graph g = m_Graph;
		Cut res;
		UnionFind uf(m_Graph.numVertices());
		while(g.numVertices() > 2) {
			auto edge = g.randomEdge();
			g.merge(edge.first, edge.second);
			uf.unite(edge.first - 1, edge.second - 1);
		}
		assert(g.numEdges() % 2 == 0);
		res.numCrossingEdges = g.numEdges() / 2;

		int root1 = uf.find(0), root2 = -1;
		res.left.push_back(1);
		for(int i = 1; i < m_Graph.numVertices(); i++) {
			int root = uf.find(i);
			if(root != root1) {
				if(root2 != -1)
					assert(root2 == root);
				else 
					root2 = root;
				res.right.push_back(i + 1);
			} else {
				res.left.push_back(i + 1);
			}
		}
		for(auto it = res.left.begin(); it != res.left.end(); ++it) {
			auto adjs = m_Graph.adjs(*it);
			for(auto it1 = adjs->begin(); it1 != adjs->end(); ++it1) {
				if(find(res.right.begin(), res.right.end(), *it1) != res.right.end()) {
					res.edges.push_back(make_pair(*it, *it1));
				}
			}

		}
		return res;
	}
	const Graph& m_Graph;
};


int main(int argc, char** argv) {
	srand(time(NULL));
	ifstream ifs(argv[1]);
	Graph graph(ifs);
	// cout << graph << endl;
	int numIt;
	istringstream iss(argv[2]);
	iss >> numIt;
	MinCut minCut(graph);
	cout << minCut.computeMinCut(numIt) << endl;
	return 0;
}
