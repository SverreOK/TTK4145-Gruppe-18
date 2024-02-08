# Compiler


ifeq ($(OS),Windows_NT) # IF WINDOWS
	CC = g++
	VCPKG_DIR = C:/dev/vcpkg_dev/vcpkg/installed/x64-windows

    INCLUDES = -I$(VCPKG_DIR)/include
    LIBS = -L$(VCPKG_DIR)/lib -lboost_system-vc140-mt -lboost_thread-vc140-mt -lboost_chrono-vc140-mt -lWs2_32

    CFLAGS = -Wall -std=c++11 -g $(INCLUDES)
else
	CC = g++
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