# Public autograder tests for EECS 280 Project 4: Editor

# Compiler and flags.  These will be inherited by recursive calls to Make.
export CXX ?= g++
export CXXFLAGS ?= --std=c++17 -Wall -pedantic -g -Wno-sign-compare -Wno-comment

# Linker and flags.  These will be inherited by recursive calls to Make.
export LD := $(CXX)
export LDFLAGS := -Wall

# Additional flags for debug, including libstdc++ debug mode and sanitizers.
# (If on macOS a.k.a. Darwin, omit libstdc++ debug mode)
# (If on CAEN Linux, omit UBSan)
UNAME := $(shell uname -s)
HOSTNAME := $(shell hostname)
ifeq "$(UNAME)" "Darwin"
	export DEBUG_FLAGS := -fsanitize=address -fsanitize=undefined
else ifneq (,$(findstring caen,$(HOSTNAME)))
	export DEBUG_FLAGS := -D_GLIBCXX_DEBUG -fsanitize=address
else
	export DEBUG_FLAGS := -D_GLIBCXX_DEBUG -fsanitize=address -fsanitize=undefined
endif

# Define DEBUG on invocation to enable debug flags, e.g.
#   make -f public.mk DEBUG=1 autograde
# WARNING - do not mix debug and regular builds. Either build them
# in separate directories or run make clean in between.
ifdef DEBUG
	CXXFLAGS += $(DEBUG_FLAGS)
	LDFLAGS += $(DEBUG_FLAGS)
endif

# "Default" recipes to build .exe and .o files.
%.exe:
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

List_tests.exe: List_tests.cpp List.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Default target runs full public autograder
test: Editor_public_test.exe line.exe
	./Editor_public_test.exe
	./line.exe < line_test1.in > line_test1.out
	diff -qB line_test1.out line_test1.out.correct
	./line.exe < line_test2.in > line_test2.out
	diff -qB line_test2.out line_test2.out.correct

line.exe: line.cpp Editor.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Editor_public_tests.exe: Editor.cpp Editor_public_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

femto.exe: femto.cpp Editor.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ -lcurses

e0.exe: e0.cpp Editor.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ -lcurses

clean:
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out
