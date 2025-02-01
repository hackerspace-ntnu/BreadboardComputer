# Variables
CC = gcc
CFLAGS = -Wall -g
LDFLAGS =

# Directories
SRCDIR = ./src/assembler
OBJDIR = ./obj
BINDIR = ./bin

# Files
SOURCES = $(SRCDIR)/assembler.c $(SRCDIR)/config.c $(SRCDIR)/lexer.c $(SRCDIR)/parser.c $(SRCDIR)/translate.c $(SRCDIR)/virtual_instruction.c
OBJECTS = $(OBJDIR)/assembler.o $(OBJDIR)/config.o $(OBJDIR)/lexer.o $(OBJDIR)/parser.o $(OBJDIR)/translate.o $(OBJDIR)/virtual_instruction.o
EXECUTABLE = $(BINDIR)/assembler.exe

# Targets
all: $(EXECUTABLE)

# Create executable
$(EXECUTABLE): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure directories exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY: all clean

# Clean up generated files
clean:
	rm -rf $(OBJDIR) $(BINDIR)
