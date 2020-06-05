# Makefile auto generated using custom generator

CPPFLAGS=-c -Wextra -std=c++14 -O$(OPT) -g
CFLAGS=-c -Wextra -O$(OPT) -g
CPPC=g++
CC=gcc
INC= -I../include\

.PHONY: all clean check-directory make-opt check-opt-value

all: check-directory

check-directory:
	@[ -d "../build/objects" ] || mkdir -p ../build/objects
	@[ -d "./" ] || mkdir -p ./
	@make SHELL=/bin/bash check-opt-value OPT=$(OPT)

check-opt-value:
	@[ "$(OPT)" == "" ] && make SHELL=/bin/bash make-opt OPT=0 || make SHELL=/bin/bash make-opt OPT=$(OPT)

make-opt:
	@if [ ! -f "../build/.out-$(OPT)" ]; then \
		rm -rf ../build/*; \
		mkdir -p ../build/objects; \
		touch ../build/.out-$(OPT); \
	fi
	@make SHELL=/bin/bash .//test-out-$(OPT) OPT=$(OPT)

.//test-out-$(OPT): ../build/objects/test-encoding.o ../build/objects/HEGMap.o ../build/objects/BinayMessageBuilder.o ../build/objects/HEGLog.o ../build/objects/HEGEncoding.o
	$(CPPC) $(INC) -o $@ $^

../build/objects/test-encoding.o: ./test-encoding.cpp ../include/HEGEncoding.hpp ../include/HEGMap.hpp ../include/BinaryMessageBuilder.hpp 
	$(CPPC) $(INC) $(CPPFLAGS) $< -o $@

../build/objects/HEGMap.o: ../src/HEGMap.cpp ../include/HEGMap.hpp 
	$(CPPC) $(INC) $(CPPFLAGS) $< -o $@

../build/objects/BinayMessageBuilder.o: ../src/BinayMessageBuilder.cpp ../include/BinaryMessageBuilder.hpp 
	$(CPPC) $(INC) $(CPPFLAGS) $< -o $@

../build/objects/HEGLog.o: ../src/HEGLog.cpp ../include/HEGLog.hpp 
	$(CPPC) $(INC) $(CPPFLAGS) $< -o $@

../build/objects/HEGEncoding.o: ../src/HEGEncoding.cpp ../include/HEGEncoding.hpp ../include/HEGDataTypes.hpp ../include/HEGLog.hpp 
	$(CPPC) $(INC) $(CPPFLAGS) $< -o $@

clean:
	rm -rf ../build .//test-out-*
