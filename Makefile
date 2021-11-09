TARGET=NestedOMP

CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -fopenmp

SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
DEPS = $(wildcard *.h) $(wildcard */*.h) $(wildcard */*/*.h)

OBJ := $(SOURCES:.cpp=.o)

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ) $(SOURCES)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LIBS) -o $@

clean:
	rm -r $(TARGET) $(OBJ)
