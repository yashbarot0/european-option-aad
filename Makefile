# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -O3 -Iinclude

# Source files
SOURCES = src/main.cpp src/EuropeanOption.cpp
HEADERS = include/EuropeanOption.hpp
OBJECTS = $(SOURCES:.cpp=.o)

# Executable
EXECUTABLE = european_option

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: all
	./$(EXECUTABLE)