# Compiler and flags
CXX = g++-14
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Iinclude -O2

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Files
# This finds all .cpp files in src/ and maps them to .o files in obj/
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/argent

# Default rule (runs when you just type 'make')
all: directories $(TARGET)

# Link all object files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compile each source file into an object file
# $< is the first prerequisite (.cpp), $@ is the target (.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories if they don't exist
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Clean up compiled files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Tell Make that these aren't actual files
.PHONY: all clean directories