#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
using std::istream;
using std::ostream;
using std::ifstream;
using std::vector;
using std::cout;
using std::endl;
const bool debug = true;
class DirectedGraph {
public:
	DirectedGraph(int i = 0) :
	_edgeIn(i),
	_edgeOut(i),
	_numEdges(0) {}
	int numVertices() const { return _edgeOut.size(); }
	int64_t numEdges() const { return _numEdges; }

	const vector<int>& getOutEdgesOf(int i) const { return _edgeOut[i]; }
	const vector<int>& getInEdgesOf(int i) const { return _edgeIn[i]; }

	void addEdge(int tail, int head) {
		_edgeIn[head].push_back(tail);
		_edgeOut[tail].push_back(head);
		_numEdges++;
	}
private:
	int _numEdges;
	vector<vector<int> > _edgeIn;
	vector<vector<int> > _edgeOut;
};
class SCC {
public:
	SCC(const DirectedGraph& g) : 
	_graph(g),
	_orderIn2ndLoop(g.numVertices()),
	_leader(g.numVertices()) {
		DFSLoop1();
		DFSLoop2();
	}

	void DFSLoop1() {
		vector<bool> explored(_graph.numVertices());
		int finishTime = 0;
		for(int i = 0; i < _graph.numVertices(); i++) {
			if(!explored[i]) {
				DFS1(i, explored, finishTime);
			}
		}
	}
	void DFS1(int i, vector<bool>& explored, int& finishTime) {
		explored[i] = true;
		const vector<int>& inEdges = _graph.getInEdgesOf(i);
		for(auto it = inEdges.begin(); it != inEdges.end(); ++it) {
			if(!explored[*it]) {
				DFS1(*it, explored, finishTime);
			}
		}
		_orderIn2ndLoop[finishTime++] = i;
	}
	void DFSLoop2() {
		vector<bool> explored(_graph.numVertices());
		for(int i = _graph.numVertices() - 1; i >= 0; --i) {
			if(!explored[_orderIn2ndLoop[i]]) {
				_sccs.push_back(vector<int>());
				DFS2(_orderIn2ndLoop[i], explored, _orderIn2ndLoop[i]);
			}
		}
	}
	void DFS2(int i, vector<bool>& explored, int leader) {
		explored[i] = true;
		_sccs.back().push_back(i);
		_leader[i] = leader;
		const vector<int>& outEdges = _graph.getOutEdgesOf(i);
		for(auto it = outEdges.begin(); it != outEdges.end(); ++it) {
			if(!explored[*it]) {
				DFS2(*it, explored, leader);
			}
		}
	}
	const vector<vector<int> > getSCCs() const { return _sccs; }
	int getLeaderOf(int i) const { return _leader[i]; }
	bool sameSCC(int i, int j) const { return _leader[i] == _leader[j]; }
private:
	const DirectedGraph& _graph;
	vector<int> _orderIn2ndLoop;
	vector<int> _leader;
	vector<vector<int> > _sccs;
};
class TwoSat{
public:
	TwoSat(istream& is) {
		is >> _numV;
		if(debug) 
			cout << "Constructing graph ..." << endl;
		DirectedGraph g(2 * _numV);
		for(int i = 0; i < _numV; i++) {
			int v1, v2;
			is >> v1 >> v2;
			g.addEdge(index(-v1), index(v2));
			g.addEdge(index(-v2), index(v1));
		}
		if(debug) 
			cout << "built graph.\nCalculating scc ..." << endl;
		SCC scc(g);
		if(debug) 
			cout << "Calculated scc.\nChecking consistence ..." << endl;
		_exist = true;
		for(int i = 1; i <= _numV; i++) {
			if(scc.sameSCC(index(i), index(-i))) {
				_exist = false;
				break;
			} 
		}
		if(debug) 
			cout << "Problem solved!" << endl;
	}
	bool exist() const { return _exist; }
private:
	int index(int i) const {
		if(i > 0)
			return i - 1;
		else {
			assert(i != 0);
			return _numV - i - 1;
		}
	}
	bool _exist;
	int _numV; //number of variables
};
int main(int argc, char** argv) {
	for(int i = 1; i < argc; i++) {
		ifstream ifs(argv[i]);
		TwoSat ts(ifs);
		cout << ts.exist() << endl;
	}	
	return 0;
}