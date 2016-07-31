#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using std::cout;
using std::ifstream;
using std::istream;
using std::vector;
using std::endl;
using std::map;
class KnapSack {
public:
	KnapSack(istream& is) {
		int n;
		is >> _W >> n;
		_values = vector<int>(n);
		_weights = vector<int>(n);
		for(int i = 0; i < n; i++) {
			is >> _values[i] >> _weights[i];
		}
		_solutions = vector<map<int,int> >(n + 1);
		for(int j = 0; j <= _W; j++) {
			_solutions[0][j] = 0;
		}
	}
	int solution(int i, int j) {
		//cout << "i = " << i << ", j = " << j;
		auto it = _solutions[i].find(j);
		if(it != _solutions[i].end()) {
			//cout << ", hit!" << endl;
			return it->second;
		}
		//cout << endl;
		int res = solution(i - 1, j);
		if(j >= _weights[i - 1]) {
			res = max(res, solution(i - 1, j - _weights[i - 1]) + _values[i - 1]);
		}
		_solutions[i][j] = res;
		return res;
	}
	int solution() { return solution(numItems(), _W); }
	size_t numItems() const { return _values.size(); }
private:
	static int max(int i, int j) { return (i > j) ? i : j; } 
	int _W;
	vector<int> _values;
	vector<int> _weights;
	vector<map<int,int> > _solutions;
};
int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	KnapSack ks(ifs);
	cout << ks.solution() << endl;
	return 0;
}