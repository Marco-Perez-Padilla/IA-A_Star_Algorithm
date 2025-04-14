CXX = g++
CXXFLAGS = -std=c++23 -Wall

SRC_DIR = src
DOCS_DIR = docs

SOURCES = $(wildcard $(SRC_DIR)/*.cc $(DOCS_DIR)/*.cc)
OBJ = $(SOURCES:.cc=.o)

EXECUTABLE = AStarAlgorithm

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXECUTABLE)
