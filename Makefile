# `make` creates the executable
# `make filename.o` creates the `filename` object file
# `make clean` will rm all object files and the executable

TARGET = solve

STD = -std=c++11 -Wno-return-type
#CXX = clang++
#WARNINGS = -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
#CXX = g++
#WARNINGS = -Wall -Wextra -Wpedantic
#SAN = -fsanitize=integer,undefined,address -g
OPTS = -O3 -march=native -flto
CXXFLAGS = $(STD) $(WARNINGS) $(OPTS) $(SAN)
LDLIBS = -pthread

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJECTS = $(SOURCES:.cpp=.o)

# linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

# compiling
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	-rm -f $(OBJECTS) $(TARGET)
