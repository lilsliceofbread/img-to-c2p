EXE_FILE := imgtoc2p
CXX := g++

OBJ_DIR := ./build/obj
BUILD_DIR := ./build

CXX_FLAGS := -Wall -O2 -I./inc
LD_FLAGS := -L./lib -lz
SOURCES := $(wildcard ./src/*.cpp)

default: obj_compile
	mkdir -p $(BUILD_DIR)
	$(CXX) $(wildcard $(OBJ_DIR)/*.o) $(CXX_FLAGS) $(LD_FLAGS) -o $(BUILD_DIR)/$(EXE_FILE)

obj_compile: 
	mkdir -p $(OBJ_DIR)
	$(CXX) -c $(SOURCES) $(CXX_FLAGS)
#   idk how to do this correctly
	mv *.o $(OBJ_DIR)

.PHONY: clean
clean:
	rm -r $(OBJ_DIR)
	rm -r $(BUILD_DIR)

simple:
    # just makes exe file in build/ with no fancy .o files
	g++ -Wall -O2 ./src/*.cpp -I./inc -L./lib -lz -o build/imgtoc2p
