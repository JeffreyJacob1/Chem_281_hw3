# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Directories
OBJDIR = obj
DEPDIR = dep
BINDIR = bin

# Libraries to link against
LIBS = -lm

# Find all source files
SOURCES = $(wildcard *.cpp)

# Find source files with a main function
MAIN_SOURCES = $(shell grep -l 'int main' $(SOURCES))

# Generate object file names
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# Generate executable names based on the source files with main
EXECUTABLES = $(MAIN_SOURCES:%.cpp=$(BINDIR)/%)

# Generate dependency file names
DEPS = $(SOURCES:%.cpp=$(DEPDIR)/%.d)

# Default target
all: directories $(EXECUTABLES)

# Create directories if they don't exist
directories:
	@mkdir -p $(OBJDIR) $(DEPDIR) $(BINDIR)

# Rule to compile source files to object files
$(OBJDIR)/%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.d,$@)

# Include dependency files
-include $(DEPS)

# Rule to link the object files into executables
$(BINDIR)/%: $(filter-out $(OBJDIR)/%_dummy.o,$(OBJECTS))
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Clean target
clean:
	@rm -rf $(OBJDIR) $(DEPDIR) $(BINDIR)

# Install target
install:
	@echo "Install commands go here"

.PHONY: all clean install directories
