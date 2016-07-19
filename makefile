CC=g++
OPT=--std=c++11
BINDIR=bin
scc_rec: scc_rec.cpp
	$(CC) $(OPT) -o $(BINDIR)/scc_rec scc_rec.cpp
scc_norec: scc_norec.cpp
	$(CC) $(OPT) -o $(BINDIR)/scc_norec scc_norec.cpp
scc_norec_o2: scc_norec.cpp
	$(CC) $(OPT) -O2 -o $(BINDIR)/scc_norec_o2 scc_norec.cpp
scc_rec_o2: scc_rec.cpp
	$(CC) $(OPT) -O2 -o $(BINDIR)/scc_rec_o2 scc_rec.cpp
dijkstra: dijkstra.cpp
	$(CC) $(OPT) -o $(BINDIR)/dijkstra dijkstra.cpp
median: median.cpp
	$(CC) $(OPT) -o $(BINDIR)/median median.cpp
jobs: jobs.cpp
	$(CC) $(OPT) -o $(BINDIR)/jobs jobs.cpp
prim: prim.cpp
	$(CC) $(OPT) -o $(BINDIR)/prim prim.cpp
