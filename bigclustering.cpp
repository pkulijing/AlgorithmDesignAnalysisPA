#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <bitset>
#include <string>
#include <algorithm>
#include <cctype>
using std::string;
using std::bitset;
using std::unordered_map;
using std::ifstream;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::vector;
const size_t MAX_NUM_BITS = 32;
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
	size_t size() const { return _leader.size() - 1; }
	unordered_map<int, vector<int> > clusters() const {
		unordered_map<int, vector<int> > _clusters;
		for(int i = 1; i <= size(); ++i) {
			_clusters[find(i)].push_back(i);
		}
		return _clusters;
	}

private:
	vector<int> _leader;
	vector<int> _size;
	int _count;
};
template <size_t N>
class HammingClustering {
public:
	HammingClustering(istream& is, size_t numNodes, size_t numBits) : _uf(numNodes), _numBits(numBits) {
		for(int i = 1; i <= numNodes; i++) {
			string s;
			std::getline(is, s);
			auto newend = std::remove(s.begin(), s.end(), ' ');
			s.erase(newend, s.end());
			_nodes.emplace_back(s);
			auto tpos = _positions.find(_nodes.back());
			if(tpos != _positions.end()) {
				_uf.unite(i, tpos->second);
				continue;
			}
			_positions[_nodes.back()] = i;
			for(int j = 0; j < numBits; j++) {
				bitset<N> jInverted = _nodes.back();
				jInverted.set(j, !jInverted.test(j));
				auto jpos = _positions.find(jInverted);
				if(jpos != _positions.end()) {
					_uf.unite(i, jpos->second);
				}
				for(int k = j + 1; k < numBits; k++) {
					bitset<N> jkInverted = jInverted;
					jkInverted.set(k, !jkInverted.test(k));
					auto jkpos = _positions.find(jkInverted);
					if(jkpos != _positions.end()) {
						_uf.unite(i, jkpos->second);
					}
				}
			}
		}
	}
	int count() const { return _uf.count(); }
	unordered_map<int, vector<int> > clusters() const { return _uf.clusters(); }
private:
	vector<bitset<N> > _nodes;
	unordered_map<bitset<N>,int> _positions;
	UnionFind _uf;
	size_t _numBits;
};
int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	size_t numNodes, numBits;
	string s;//to address the first \n
	ifs >> numNodes >> numBits;
	std::getline(ifs, s);
	assert(numBits <= MAX_NUM_BITS); 
	HammingClustering<MAX_NUM_BITS> hc(ifs, numNodes, numBits);
	cout << hc.count() << endl;
	// auto clusters = hc.clusters();
	// for(auto it = clusters.begin(); it != clusters.end(); it++) {
	// 	for(auto itt = it->second.begin(); itt != it->second.end(); itt++) {
	// 		cout << *itt << " ";
	// 	}
	// 	cout << "\n";
	// }
	return 0;
}
