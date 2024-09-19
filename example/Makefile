# Define the compiler
CC = gcc

# Define the flags for the compiler
CFLAGS = -Wall -g

# Define the target executable name
TARGET = main

# Define the source files
SRCS = main.c

# Define the object files
OBJS = $(SRCS:.c=.o)

# Define the library to link with
LIBS = -L. ../lwut.a -lm -lGL -lGLU $(shell pkg-config --libs sdl2 SDL2_image) 

# Default target to build
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I../source

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

