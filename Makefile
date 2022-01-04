INCLUDES=-Iinclude -I$${HOME}/Code/cpp_workbench/anvil/include
LIB_LOCATIONS=-L$${HOME}/Code/cpp_workbench/anvil/build/lib
LIBS=-lanvil
CC=clang++ -g -O0 -std=c++17 
CPPFLAGS=-Wall -Wextra $(INCLUDES) 

SRC_DIR := ./src
OBJ_DIR := build/obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

.PHONY: dirs

default: dirs
	$(MAKE) main

dirs:
	mkdir -p build/obj build/bin

clean:
	rm -rf build *.tga

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

main: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LIB_LOCATIONS) $(LIBS) build/obj/*.o -o build/bin/main 

