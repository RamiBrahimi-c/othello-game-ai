# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Targets
TARGET = $(BINDIR)/main
SOURCES = $(SRCDIR)/Game.c $(SRCDIR)/GameAi.c $(SRCDIR)/main.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Create executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BINDIR) $(OBJDIR):
	mkdir -p $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Phony targets
.PHONY: all clean run debug