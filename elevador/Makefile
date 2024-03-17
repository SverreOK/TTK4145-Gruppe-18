# Compiler
CC = g++

# Compiler flags
CFLAGS = -Iinc -Iinc/data -Iinc/elevator -Iinc/util -Iinc/network -Iinc/startup
# Extra compiler flags for more error hehehehehe
CFLAGS += -Wall -Wextra -Wpedantic

# Debug compiler flags
CFLAGS_DEBUG = -g $(CFLAGS)

# Linker flags
LDFLAGS = -ljsoncpp -lpthread -lboost_system -lboost_thread -lboost_chrono -lncurses

# Source filesm
SRC = $(wildcard src/*.cpp src/data/*.cpp src/elevator/*.cpp src/network/*.cpp src/startup/*.cpp src/util/*.cpp)

# Object files
OBJ = $(SRC:.cpp=.o)

# Debug object files
OBJ_DEBUG = $(SRC:.cpp=.debug.o)

# Executable name
EXEC = main

# Debug executable name
EXEC_DEBUG = main_debug

all: $(EXEC)

debug: $(EXEC_DEBUG)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(EXEC_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CFLAGS_DEBUG) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.debug.o: %.cpp
	$(CC) $(CFLAGS_DEBUG) -g -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJ_DEBUG) $(EXEC) $(EXEC_DEBUG)

dirty: $(EXEC)