# Detect OS
OS := $(shell uname -s)

# Compiler and linker settings for both Linux and Windows
ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(OS)
endif

ifeq ($(detected_OS),Windows)
    CC = cl
    LINK = link
    BOOST_PATH = C:/local/boost_1_84_0
    CFLAGS = /EHsc /std:c++11 /Iinc /I"$(BOOST_PATH)"
    LFLAGS = /LIBPATH:"$(BOOST_PATH)"
    LIBS = boost_system-vc141-mt-gd.lib boost_thread-vc141-mt-gd.lib
    OBJ_EXT = .obj
    EXE_EXT = .exe
    RM = del /Q
    FIX_PATH = $(subst /,\,$1)
else
    CC = g++
    CFLAGS = -Wall -std=c++11 -g -Iinc
    LFLAGS =
    LIBS = -lboost_system -lboost_thread -lpthread
    OBJ_EXT = .o
    EXE_EXT =
    RM = rm -f
    FIX_PATH = $1
endif

# Target executable name
TARGET = main$(EXE_EXT)

# Object files
OBJS = main$(OBJ_EXT) elevator_driver$(OBJ_EXT)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(detected_OS),Windows)
    $(LINK) $(LFLAGS) /OUT:$(TARGET) $(OBJS) $(LIBS)
else
    $(CC) -o $(TARGET) $(OBJS) $(LIBS)
endif

main$(OBJ_EXT): main.cpp inc/elevator_driver.h
    $(CC) $(CFLAGS) -c main.cpp /Fo$@

elevator_driver$(OBJ_EXT): src/elevator_driver.cpp inc/elevator_driver.h
    $(CC) $(CFLAGS) -c src/elevator_driver.cpp /Fo$@

clean:
    $(RM) $(call FIX_PATH,$(TARGET) $(OBJS))

.PHONY: all clean