CXX=g++
CXXFLAGS=-std=c++14 -Wall -g -fpermissive -pthread -O3 -MD -MP
SRC_DIR=src
OBJ_DIR=obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.cc)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=-I./libs/nlohmann -lcurl

sim: main.o

main.o: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o build/sim.out $(OBJ_FILES) $(INCLUDE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(INCLUDE)

-include $(SRC_FILES:%.cc=%.d)

.PHONY: clean
clean: 
	rm $(wildcard obj/*.o)
	rm $(wildcard obj/*.d)
	rm build/sim.out
all:
	make clean
	make
