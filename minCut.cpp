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
using namespace std;
class Graph;
class MinCut;
class UnionFind;
struct Cut;
class Graph {
public:
	Graph(istream& is) : m_NumEdges(0) {
		string line;
		while(getline(is, line)) {
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
	return os;
}
class MinCut {
public:
	MinCut(const Graph& G):m_Graph(G) {}
	Cut computeMinCut(int numIt) {
		cout << "Start ..." << endl;
		Cut best = randomContract();
		for(int i = 0; i < numIt; ++i) {
			cout << "Iterator = " << i << ".";
			Cut res = randomContract();
			if(res.numCrossingEdges < best.numCrossingEdges)
				best = res;
			cout << "Iterator = " << i << ", res = " << res.numCrossingEdges << ", best = " << best.numCrossingEdges << endl;
		}
		return best;
	} 
private:
	Cut randomContract() const {
		Graph g = m_Graph;
		Cut res;
		vector<pair<int,int> > ps = {make_pair(3,2), make_pair(4,2)};
		int i = 0;
		while(g.numVertices() > 2) {
			auto edge = g.randomEdge();
			//auto edge = ps[i++];
			// cout << "edge = " << edge.first << "-" << edge.second << endl; 
			g.merge(edge.first, edge.second);
			// cout << g << endl;
		}
		assert(g.numEdges() % 2 == 0);
		res.numCrossingEdges = g.numEdges() / 2;
		return res;
	}
	const Graph& m_Graph;
};


int main(int argc, char** argv) {
	srand(time(NULL));
	ifstream ifs(argv[1]);
	Graph graph(ifs);
	cout << graph << endl;
	MinCut minCut(graph);
	cout << minCut.computeMinCut(60) << endl;
	return 0;
}
