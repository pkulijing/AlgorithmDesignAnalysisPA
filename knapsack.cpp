#include <iostream>
#include <fstream>
#include <vector>
using std::cout;
using std::ifstream;
using std::istream;
using std::vector;
using std::endl;
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
		_solutions = vector<vector<int> >(n + 1, vector<int>(_W + 1));
		for(int j = 0; j <= _W; j++) {
			_solutions[0][j] = 0;
		}
		for(int i = 1; i <= n; i++) {
			for(int j = 0; j <= _W; j++) {
				if(j >= _weights[i - 1]) {
					_solutions[i][j] = max(_solutions[i - 1][j], 
						_solutions[i - 1][j - _weights[i - 1]] + _values[i - 1]);
				} else {
					_solutions[i][j] = _solutions[i - 1][j];
				}
			}
		}
	}
	int solution() const { return _solutions[numItems()][_W]; }
	size_t numItems() const { return _values.size(); }
private:
	static int max(int i, int j) { return (i > j) ? i : j; } 
	int _W;
	vector<int> _values;
	vector<int> _weights;
	vector<vector<int> > _solutions;
};
int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	KnapSack ks(ifs);
	cout << ks.solution() << endl;
	return 0;
}