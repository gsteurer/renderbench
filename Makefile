INCLUDES=-I$${HOME}/Code/cpp_workbench/anvil/include
LIB_LOCATIONS=-L$${HOME}/Code/cpp_workbench/anvil/build/lib
LIBS=-lanvil
CC=clang++ -g -O0 -std=c++17 
CPPFLAGS=-Wall -Wextra $(INCLUDES) 

.PHONY: dirs

default: dirs
	$(MAKE) main

dirs:
	mkdir -p build/obj build/bin

clean:
	rm -rf build *.spv

build/obj/main.o: src/main.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

main: build/obj/main.o
	$(CC) $(CPPFLAGS) $(LIB_LOCATIONS) $(LIBS) $(FRAMEWORKS) -o build/bin/$@ $^