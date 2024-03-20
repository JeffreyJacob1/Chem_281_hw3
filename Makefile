# Compiler
CPP = g++

# Compiler flags
CPPFLAGS = -O3 -std=c++11 

1: 
	$(CPP) $(CPPFLAGS) -o 1 2_part1.cpp

2:
	$(CPP) $(CPPFLAGS) -o 2 2_part2.cpp 

4:
	g++ -std=c++11 -O3 -DARMA_DONT_USE_WRAPPER -I eigen-3.4.0/ 2_part4.cpp -o 4 -larmadillo

5:
	g++ -std=c++11 -march=native -O3 2_part5.cpp -o 5




all: 1 2 4 5

clean:
	rm 1 2 4 5