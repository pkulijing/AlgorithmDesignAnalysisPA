#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <climits>
#include <unordered_map>
#include <chrono>
#include <bitset>
#include <cfloat>
using std::unordered_map;
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::ostream;
using std::istream;
using std::bitset;
using std::min;
const int N = 25;
class TSP {
public:
	TSP(const vector<float>& x, const vector<float>& y) 
		: _x(x),
		  _y(y), 
		  _distance(x.size(), vector<float>(x.size(), 0)),
		  _nChooseK(buildNChooseK(x.size() - 1)) {
		assert(y.size() == x.size());
		size_t n = x.size();
		for(int i = 0; i < n; i++) {
			for(int j = i + 1; j < n; j++) {
				_distance[i][j] = dist(x[i], y[i], x[j], y[j]);
			}
		}
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < i; j++) {
				_distance[i][j] = _distance[j][i];
			}
		}
		//A greedy solution
		vector<bool> used(n, false);
		used[0] = true;
		int cur = 0;
		float threshold = 0;
		for(int i = 1; i < n; i++) {
			float minV = FLT_MAX;
			int choice = -1;
			for(int j = 1; j < n; j++) {
				if(!used[j] && _distance[j][cur] < minV) {
					minV = _distance[j][cur];
					choice = j;
				}
			}
			cur = choice;
			used[choice] = true;
			threshold += minV;
		}
		threshold += _distance[cur][0];
		cout << threshold << endl;
		unordered_map<int, vector<float> > minLens;
		bitset<N> bs(0);
		for(int j = 0; j < n - 1; j++) {
			bitset<N> bs1 = bs;
			bs1.set(j);
			int num1 = bs1.to_ulong();
			vector<float> t(n - 1, threshold + 1);
			t[j] = _distance[0][j + 1];
			minLens[num1] = t;
		}
		for(int i = 3; i <= n; i++) {
			unordered_map<int, vector<float> > newMinLens;
			newMinLens.reserve(_nChooseK[i - 1]);
			for(auto it = minLens.begin(); it != minLens.end(); it++) {
				int num = it->first;
				bitset<N> bs(num);
				for(int j = 0; j < n - 1; j++){
					if(!bs[j]) {
						bitset<N> bs1 = bs;
						bs1.set(j);
						int num1 = bs1.to_ulong();
						float val = threshold + 1;
						for(int k = 0; k < n - 1; k++) {
							if(k == j)
								continue;
							val = min(val, it->second[k] + _distance[k + 1][j + 1]);
						}
						auto itFind = newMinLens.find(num1);
						if(itFind == newMinLens.end()) {
							auto itInsert = newMinLens.emplace(num1, vector<float>(n - 1, threshold + 1));
							itInsert.first->second[j] = val;
						} else {
							itFind->second[j] = val;				
						}	
					}
				}
			}
			minLens.swap(newMinLens);
			cout << "i = " << i << ", num of subsets = " << minLens.size() << endl;
		}
		assert(minLens.size() == 1);
		_minLen = threshold + 1;
		auto it = minLens.begin();
		for(int k = 0; k < n - 1; k++) {
			_minLen = min(_minLen, it->second[k] + _distance[k + 1][0]);
			cout << it->second[k] << endl;
		}
	}
	float result() const { return _minLen; }
private:
	float _minLen;
	vector<float> _x;
	vector<float> _y;
	vector<vector<float> > _distance;
	static float dist(float x1, float y1, float x2, float y2) {
		float dx = x1 - x2, dy = y1 - y2;
		return sqrt(dx * dx + dy * dy);
	}
	static vector<int> buildNChooseK(int n) {
		vector<int> res(n + 1, 0);
		res[0] = 1;
		for(int i = 1; i <= n; i++) {
			if(n - i < i) {
				res[i] = res[n - i];
			} else {
				res[i] = res[i - 1] * (n + 1 - i) / i;
			}
		}
		return res;
	}
	vector<int> _nChooseK;
};
int main(int argc, char** argv) {
	using std::chrono::system_clock;
	auto t0 = system_clock::now();
	ifstream ifs(argv[1]);
	int n;
	ifs >> n;
	vector<float> xs, ys;
	for(int i = 0; i < n; i++) {
		float x,y;
		ifs >> x >> y;
		xs.push_back(x);
		ys.push_back(y);
	}
	TSP tsp(xs, ys);
	cout << tsp.result() << endl;
	auto t1 = system_clock::now();
	cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(t1 - t0).count() << " seconds." << endl;
	return 0;
}