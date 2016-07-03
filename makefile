scc_rec: scc_rec.cpp
	g++ --std=c++11 -o bin/scc_rec scc_rec.cpp
scc_norec: scc_norec.cpp
	g++ --std=c++11 -o bin/scc_norec scc_norec.cpp
scc_norec_o2: scc_norec.cpp
	g++ --std=c++11 -O2 -o bin/scc_norec_o2 scc_norec.cpp
scc_rec_o2: scc_rec.cpp
	g++ --std=c++11 -O2 -o bin/scc_rec_o2 scc_rec.cpp
all: scc_norec.cpp scc_rec.cpp
	g++ --std=c++11 -o bin/scc_rec scc_rec.cpp
	g++ --std=c++11 -o bin/scc_norec scc_norec.cpp
	g++ --std=c++11 -O2 -o bin/scc_norec_o2 scc_norec.cpp
	g++ --std=c++11 -O2 -o bin/scc_rec_o2 scc_rec.cpp
