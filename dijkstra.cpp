#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <cassert>
#include <string>
using std::cout;
using std::vector;
using std::ifstream;
using std::istream;
using std::ostream;
using std::istringstream;
using std::string;
using std::map;
using std::endl;
const int MAX_LENGTH = 1000000;
class Graph {
public:
	Graph(istream& is) : m_Edges(1), m_EdgeLengths(1), m_NumEdges(0) {
		string line;
		int v = 1;
		while(std::getline(is, line)) {
			if(line.empty())
				continue;
			istringstream iss(line);
			int i, vertex, length;
			char comma;
			iss >> i;
			assert(i == v++);
			m_Edges.push_back(vector<int>());
			m_EdgeLengths.push_back(vector<int>());
			while(iss >> vertex) {
				iss >> comma >> length;
				assert(comma == ',');
				m_Edges[i].push_back(vertex);
				m_EdgeLengths[i].push_back(length);
			}
			m_NumEdges += m_Edges[i].size();
		}
	}
	int numVertices() const { return m_Edges.size() - 1; }
	int numEdges() const { return m_NumEdges; }
	const vector<int>& getEdgesOf(int i) const { return m_Edges[i]; }
	const vector<int>& getEdgeLengthsOf(int i) const { return m_EdgeLengths[i]; }
	void print(ostream& os) const {
		for(int i = 1; i <= numVertices(); i++) {
			os << i << ":";
			for(int j = 0; j < m_Edges[i].size(); j++) {
				os << " " << m_Edges[i][j] << "," << m_EdgeLengths[i][j];
			}
			os << "\n";
		}
	}
private:
	int m_NumEdges;
	vector<vector<int> > m_Edges;
	vector<vector<int> > m_EdgeLengths;
};
ostream& operator<<(ostream& os, const Graph& g) {
	g.print(os);
	return os;
}
template <typename T>
class Heap {
public:
	struct Node {
		Node() = default;
		Node(const T& i, int k) : index(i), key(k) {}
		T index;
		int key;
	};
	Heap() {
		//We don't use index 0
		m_Nodes.push_back(Node(-1,-1));
	}
	void add(const Node& node) {
		assert(m_LookUpTable.find(node.index) == m_LookUpTable.end());
		m_Nodes.push_back(node);
		m_LookUpTable[node.index] = m_Nodes.size() - 1;
		bubble(m_Nodes.size() - 1);
	}
	Node getMin() {
		Node n = m_Nodes[1];
		m_LookUpTable.erase(n.index);
		m_Nodes[1] = m_Nodes.back();
		m_Nodes.resize(m_Nodes.size() - 1);
		sink(1);
		return n;
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
			//j should be the smaller one of the two sons
			if(hasRightSon(i) && less(j + 1, j))
				j++;
			if(!less(j, i))
				break;
			swap(i, j);
			i = j;
		}
	}
	void bubble(int i) {
		while(hasParent(i) && less(i, i / 2)) {
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
	bool less(int i, int j) {
		return m_Nodes[i].key < m_Nodes[j].key;
	}
	vector<Node> m_Nodes;
	map<T,int> m_LookUpTable;
};
class Dijkstra {
public:
	Dijkstra(const Graph& G, int s = 1) : m_G(G), m_Source(s),
	 m_MinLengths(G.numVertices() + 1), m_Paths(G.numVertices() + 1) {
		for(int i = 1; i <= m_G.numVertices(); i++) {
			int len = (i == m_Source) ? 0 : MAX_LENGTH;				
			m_Heap.add(Heap<int>::Node(i, len));
		}
		vector<int> previous(m_G.numVertices() + 1);
		while(!m_Heap.empty()) {
			Heap<int>::Node n = m_Heap.getMin();
			m_MinLengths[n.index] = n.key;
			if(n.index != m_Source) {
				m_Paths[n.index] = m_Paths[previous[n.index]];
				m_Paths[n.index].push_back(n.index);
			}
			const vector<int>& edges = m_G.getEdgesOf(n.index);
			const vector<int>& edgeLengths = m_G.getEdgeLengthsOf(n.index);
			assert(edgeLengths.size() == edges.size());
			for(int i = 0; i < edges.size(); i++) {
				if(m_Heap.contains(edges[i]) && n.key + edgeLengths[i] < m_Heap.getKey(edges[i])) {
					m_Heap.changeKey(edges[i], n.key + edgeLengths[i]);
					previous[edges[i]] = n.index;
				}
			}
		}
	}
	int minLengthFor(int i) const { return m_MinLengths[i]; }
	const vector<int>& pathFor(int i) const { return m_Paths[i]; }
private:
	Heap<int> m_Heap;
	vector<int> m_MinLengths;
	vector<vector<int> > m_Paths;
	const Graph& m_G;
	const int m_Source;
};
int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	Graph g(ifs);
	//cout << g << endl;
	Dijkstra shortestPath(g, 1);
	for(int i = 1; i <= g.numVertices(); i++) {
		cout << i << ": " << shortestPath.minLengthFor(i) << " [";
		const vector<int>& path = shortestPath.pathFor(i);
		for(auto it = path.begin(); it != path.end(); ++it) {
			cout << (it == path.begin() ? "" : ", ") << *it; 
		}
		cout << "]" << endl;
	}
	vector<int> requested = {7,37,59,82,99,115,133,165,188,197};
	for(int i = 0; i < requested.size(); i++) {
		cout << (i ? "," : "") << shortestPath.minLengthFor(requested[i]);
	}
	cout << endl;
	return 0;
}
