#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <climits>
#include <chrono>
#include <bitset>
#include <cfloat>
#include <algorithm>
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
static int power(int a, int n) {
 	int res = 1;
 	for(int i = 0; i < n; i++) {
 		res *= a;
 	}
 	return res;
}
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
		vector<float> minLens(power(2, n - 1) * (n - 1), threshold + 1);
		bitset<N> bs(0);
		for(int j = 0; j < n - 1; j++) {
			bitset<N> bs1 = bs;
			bs1.set(j);
			int num1 = bs1.to_ulong();
			minLens[num1 * (n - 1) + j] = _distance[0][j + 1];
		}
		for(int i = 3; i < n; i++) {
			vector<int> indices = permutations(n - 1, i - 1);
			for(auto it = indices.begin(); it != indices.end(); it++) {
				bitset<N> bs(*it);
				for(int j = 0; j < n - 1; j++) {
					if(!bs[j]) {
						bitset<N> bs1 = bs;
						bs1.flip(j);
						float val = threshold + 1;
						for(int k = 0; k < n - 1; k++) {
							if(k == j)
								continue;
							val = min(val, minLens[*it * (n - 1) + k] + _distance[k + 1][j + 1]);
						}
						minLens[bs1.to_ulong() * (n - 1) + j] = val;
					}
				}
			}
			cout << "i = " << i << ", num of subsets = " << indices.size() << endl;
		}
		_minLen = threshold + 1;
		size_t sz = minLens.size();
		for(int k = 0; k < n - 1; k++) {
			_minLen = min(_minLen, minLens[sz - (n - 1) + k] + _distance[k + 1][0]);
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
	static vector<int> permutations(int n, int k) {
		vector<int> res;
		vector<bool> bv(n - k, 0);
		bv.resize(n, 1);
		do {
			bitset<32> bs(0);
			for(int i = 0; i < n; i++) {
				if(bv[i])
					bs.set(n - 1 - i);
			}
			res.push_back(bs.to_ulong());
		} while (next_permutation(bv.begin(), bv.end()));
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