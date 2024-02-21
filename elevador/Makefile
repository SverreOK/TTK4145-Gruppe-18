# Compiler
CC = g++

# Compiler flags
CFLAGS = -Iinc -Iinc/data -Iinc/elevator

# Source files
SRC = $(wildcard src/*.cpp src/data/*.cpp src/elevator/*.cpp)

# Object files
OBJ = $(SRC:.cpp=.o)

# Executable name
EXEC = my_program

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)