# Makefile auto generated using custom generator

CFLAGS=-c -Wextra -std=c++17 -O$(OPT) -g
CC=g++
INC= -I.//include\

.PHONY: all clean cleanall check-directory make-opt check-opt-value

all: check-directory

check-directory:
	@[ -d "build/objects" ] || mkdir -p build/objects
	@[ -d "build" ] || mkdir -p build
	@make SHELL=/bin/bash check-opt-value OPT=$(OPT)

check-opt-value:
	@[ "$(OPT)" == "" ] && make SHELL=/bin/bash make-opt OPT=0 || make SHELL=/bin/bash make-opt OPT=$(OPT)

make-opt:
	@if [ ! -f "build/.out-$(OPT)" ]; then \
		rm -rf build/*; \
		mkdir -p build/objects; \
		touch build/.out-$(OPT); \
	fi
	@make SHELL=/bin/bash build/test-out-$(OPT) OPT=$(OPT)
	@make SHELL=/bin/bash build/prj-out-$(OPT) OPT=$(OPT)

build/test-out-$(OPT): build/objects/test-decoding.o build/objects/HEGMap.o build/objects/HEGEncoding.o
	$(CC) $(INC) -o $@ $^

build/prj-out-$(OPT): build/objects/HEGMap.o build/objects/prj-main.o build/objects/HEGEncoding.o
	$(CC) $(INC) -o $@ $^

build/objects/test-decoding.o: .//tests/test-decoding.cpp .//include/HEGMap.hpp .//include/HEGEncoding.hpp 
	$(CC) $(INC) $(CFLAGS) $< -o $@

build/objects/HEGMap.o: .//src/HEGMap.cpp .//include/HEGMap.hpp 
	$(CC) $(INC) $(CFLAGS) $< -o $@

build/objects/prj-main.o: .//src/prj-main.cpp .//include/HEGDataTypes.hpp .//include/HEGEncoding.hpp 
	$(CC) $(INC) $(CFLAGS) $< -o $@

build/objects/HEGEncoding.o: .//src/HEGEncoding.cpp .//include/HEGEncoding.hpp .//include/HEGDataTypes.hpp 
	$(CC) $(INC) $(CFLAGS) $< -o $@

clean:
	rm -rf build

cleanall:
	rm -rf build data/*.txt