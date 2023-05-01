# idk how to do good makefiles lol
EXE_FILE := imgtoc2p
CXX := g++

OBJ_DIR := ./obj
BIN_DIR := ./bin

COMP_FLAGS := -g -I../inc -c # -c ensures compile to .o files (CHANGE -g TO -O2 WHEN PROJ DONE)
LINK_FLAGS := -L../lib -lz # if this doesn't work try moving -L to COMP_FLAGS
SOURCES := ../src/$(wildcard *.cpp)

default: obj_compile
	mkdir ./bin
	$(CXX) $(BIN_DIR)/*.o -o $(BIN_DIR)/$(EXE_FILE) $(LINK_FLAGS)

obj_compile: 
	mkdir ./obj
	$(CXX) $(SOURCES) $(COMP_FLAGS)
	mv *.o ./obj

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

simple: # just makes exe file in build/ with no fancy .o files
	g++ -g ../src/*.cpp -I../inc -L../lib -lz -o imgtoc2p