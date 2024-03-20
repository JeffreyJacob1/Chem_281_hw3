# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Directories
OBJDIR = obj
DEPDIR = dep
BINDIR = bin

# Libraries to link against
LIBS = -lm

# Target executable
TARGET = $(BINDIR)/main

# Default target
all: $(TARGET)

# Find all source files
SOURCES = $(wildcard *.cpp)

# Generate object file names
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Generate dependency file names
DEPS = $(patsubst %.cpp,$(DEPDIR)/%.d,$(SOURCES))

# Create directories if they don't exist
$(OBJDIR) $(DEPDIR) $(BINDIR):
	mkdir -p $@

# Rule to generate object files
$(OBJDIR)/%.o: %.cpp | $(OBJDIR) $(DEPDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MP -MF $(DEPDIR)/$*.d

# Include dependency files
-include $(DEPS)

# Rule to link the object files into the target executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBS)

# Clean target
clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(BINDIR)

# Install target
install:
	# Add installation commands here

.PHONY: all clean install