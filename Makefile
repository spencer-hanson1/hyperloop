## UNSW Hyperloop
## Software Screen Task
## Spencer Hanson
## Makefile

# INSTRUCTIONS:
#		1. $ make main
#		2. $ ./main < stdin
#		3. $ make clean

# TODO
EXECUTABLE  = main
DEBUG       = $(EXECUTABLE)_debug

# TODO
CXX         = g++
CXXFLAGS    = -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic -pthread

# TODO
PROJECTFILE = main.cpp

# list of sources used in project
SOURCES     = $(wildcard *.cpp)

# list of objects used in project
OBJECTS     = $(SOURCES:%.cpp=%.o)

# make release - will compile "all" with $(CXXFLAGS) and the -O3 flag
#                also defines NDEBUG so that asserts will not check
release: CXXFLAGS += -O3 -DNDEBUG
release: $(EXECUTABLE)

# make debug - will compile "all" with $(CXXFLAGS) and the -g flag
#              also defines DEBUG, so "#ifdef DEBUG /*...*/ #endif" works
debug: EXECUTABLE := $(DEBUG)
debug: CXXFLAGS += -g3 -DDEBUG
debug: clean $(EXECUTABLE)

# make clean - remove .o files, executables, tarball
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DEBUG)
	rm -Rf *.dSYM

$(EXECUTABLE): $(OBJECTS)
ifeq ($(EXECUTABLE), executable)
	@echo Edit EXECUTABLE variable in Makefile.
	@echo Using default a.out.
	$(CXX) $(CXXFLAGS) $(OBJECTS)
else
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)
endif

# rule for creating objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

define MAKEFILE_HELP
Spencer's Makefile Help
* General usage:
	1. $ make main
	2. $ ./main < stdin
	3. $ make clean

* For more information:
	$ man make
endef
export MAKEFILE_HELP

help:
	@echo "$$MAKEFILE_HELP"

# these targets do not create any files
.PHONY: release debug clean help
# disable built-in rules
.SUFFIXES:
