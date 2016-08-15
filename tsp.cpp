#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdio>
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::ostream;
using std::istream;
int main(int argc, char** argv) {
	ifstream ifs(argv[1]);
	int n;
	ifs >> n;
	for(int i = 0; i < n; i++) {
		double x,y;
		ifs >> x >> y;
		printf("%.4lf %.4lf\n", x, y);
	}
	return 0;
}