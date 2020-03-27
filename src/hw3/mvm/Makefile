CXX=g++
CXXFLAGS=-O4 -fno-tree-vectorize -mavx2 -mfma 

main: main.o kernel.o
	$(CXX) -o main main.o kernel.o 
