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
clustering: clustering.cpp
	$(CC) $(OPT) -o $(BINDIR)/clustering clustering.cpp
bigclustering: bigclustering.cpp
	$(CC) $(OPT) -o $(BINDIR)/bigclustering bigclustering.cpp
knapsack: knapsack.cpp
	$(CC) $(OPT) -o $(BINDIR)/knapsack knapsack.cpp
knapsack_big: knapsack_big.cpp
	$(CC) $(OPT) -o $(BINDIR)/knapsack_big knapsack_big.cpp
optimalBST: optimalBST.cpp
	$(CC) $(OPT) -o $(BINDIR)/optimalBST optimalBST.cpp

