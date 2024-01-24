# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -std=c++11

# Libraries for linking
LIBS = -lboost_system -lboost_thread -lpthread

# Target executable
TARGET = elevator

# Object files
OBJS = main.o elevator_driver.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

main.o: main.cpp elevator_driver.h
	$(CC) $(CFLAGS) -c main.cpp

elevator_driver.o: elevator_driver.cpp elevator_driver.h
	$(CC) $(CFLAGS) -c elevator_driver.cpp

clean:
	$(RM) $(TARGET) $(OBJS)