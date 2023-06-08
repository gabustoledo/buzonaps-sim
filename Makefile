CXX=g++
CXXFLAGS=-std=c++14 -Wall -g -fpermissive -pthread -O3

sim: main.o

main.o: patient.o
	$(CXX) $(CXXFLAGS) src/main.cc -o build/sim.out $(wildcard obj/*.o)

patient.o:
	$(CXX) $(CXXFLAGS) src/Patient.cc -c -o obj/patient.o $(INCLUDE)

.PHONY: clean
clean: 
	rm $(wildcard obj/*.o)
	rm build/sim.out

all:
	make clean
	make