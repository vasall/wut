# Name of the created library
TARGET     := lib_fhgraphic.a

# Get the absolute path to the directory this makefile is in
MKFILE_PTH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(dir $(MKFILE_PTH))

# The compiler to use
CC         := gcc
# Error flags for compiling
ERRFLAGS   := -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
# Compiling flags here
CFLAGS     := -g -O0 -ansi -std=c89 -I. -I./inc/ -I.. -pedantic -D_POSIX_C_SOURCE=200809L
SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_image)
override CFLAGS += $(SDL_CFLAGS)

# The linker to use
LINKER     := gcc
# Set libararies (FIXME: switch the variables)
LIBS  := -lm -lGL -lGLU
# Linking flags here
LFLAGS     := -Wall -I. $(LIBS)
SDL_LFLAGS  := $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_image)
override LFLAGS += $(SDL_LFLAGS)

# Change these to proper directories where each file should be
SRCDIR     := src
OBJDIR     := .

SOURCES    := $(wildcard $(SRCDIR)/*.c)
INCLUDES   := $(wildcard $(SRCDIR)/*.h)
OBJECTS    := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm         := rm -f

$(TARGET): $(OBJECTS)
	@ar -r -o $@ $(OBJECTS)
	@echo "Linking "$@" complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) $(ERRFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

# Create the directories to store the object-files and the final binary
.PHONY: dirs
dirs:
	mkdir -p obj


