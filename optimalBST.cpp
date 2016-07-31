#include <iostream>
#include <fstream>
#include <vector>
using std::cout;
using std::ifstream;
using std::ostream;
using std::istream;
using std::vector;
using std::endl;
class OptimalBST {
public:
	OptimalBST(istream& is) {
		double pi;
		while(is >> pi) {
			_p.push_back(pi);
		}
		_solution = vector<vector<double> >(_p.size(), vector<double>(_p.size()));
		compute();
	}
	OptimalBST(const vector<double>& p) : _p(p), _solution(p.size(), vector<double>(p.size())){
		compute();
	}
	size_t size() const { return _p.size(); }
	double solution() const { return _solution[0][size() - 1]; }
	void print(ostream& os) const {
		for(int j = 0; j < size(); j++) {
			for(int i = 0; i < size(); i++) {
				os << _solution[i][j] << '\t';
			}
			os << endl;
		}
	}
private:
	void compute() {
		for(int i = 0; i < size(); i++) {
			_solution[i][i] = _p[i];
			for(int j = 0; j < i; j++) {
				_solution[i][j] = 0;
			}
		}
		for(int s = 1; s < size(); s++) {
			for(int i = 0; i + s < size(); i++) {
				double res = _solution[i + 1][i + s];
				for(int r = i + 1; r < i + s; r++) {
					res = min(res, _solution[i][r - 1] + _solution[r + 1][i + s]);
				}
				res = min(res, _solution[i][i + s - 1]);
				_solution[i][i + s] = res + psum(i, i + s);
			}
		}
	}
	double psum(int i, int j) {
		double res = 0;
		for(int k = i; k <= j; k++) {
			res += _p[k];
		}
		return res;
	}
	static double min(double a, double b) { return (a > b) ? b : a; }
	vector<double> _p;
	vector<vector<double> > _solution;
};
int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	OptimalBST obst(ifs);
	cout << obst.solution() << endl;
	obst.print(cout);
	return 0;
}