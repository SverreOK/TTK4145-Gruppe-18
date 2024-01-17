# need this -lboost_system -lpthread

# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra

# Source files
SRCS = tcp_client.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executables
TARGETS = $(SRCS:.cpp=)

# Default target
all: $(TARGETS)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the executables
tcp_client: tcp_client.o
	$(CC) $(CFLAGS) $< -o $@ -lboost_system -lpthread

# Clean up object files and the executables
clean:
	rm -f $(OBJS) $(TARGETS)