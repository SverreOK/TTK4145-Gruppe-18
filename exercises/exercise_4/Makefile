# Makefile for compiling a project with main.cpp and process_pair.h

CXX = g++
CXXFLAGS = -Wall -std=c++14
# Adjust the path below to your Boost installation if necessary
BOOST_FLAGS = -I /usr/include/boost
LIBS = -lboost_system -lpthread

# The name of your executable
TARGET = main

# Source files to compile
SOURCES = main.cpp

# Object files correspond to each source file
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(BOOST_FLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(BOOST_FLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
