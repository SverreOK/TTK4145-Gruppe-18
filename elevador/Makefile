# Compiler
CC = g++

ifeq ($(OS),Windows_NT) # IF WINDOWS
    BOOST_PATH = C:/local/boost_1_84_0

    CFLAGS = -Wall -std=c++11 -g -Iinc -I$(BOOST_PATH)

	LIBS = -L$(BOOST_PATH)

else
	LIBS = -lboost_system -lboost_thread -lpthread

	CFLAGS = -Wall -std=c++11 -g -Iinc
endif

# Target executable
TARGET = main

# Object files
OBJS = main.o elevator_driver.o


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

main.o: main.cpp inc/elevator_driver.h
	$(CC) $(CFLAGS) -c main.cpp

elevator_driver.o: src/elevator_driver.cpp inc/elevator_driver.h
	$(CC) $(CFLAGS) -c src/elevator_driver.cpp

clean:
	$(RM) $(TARGET) $(OBJS)