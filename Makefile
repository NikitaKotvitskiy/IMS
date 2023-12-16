# Makefile

# Compiler and flags
CC := g++
CFLAGS := -std=c++11

# Source files and object files
SRC_DIR := sources
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, %.o, $(SRCS))

# Target executable
TARGET := mcd

# Libraries
LIBS := -lsimlib -lm

# Build rule for the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Build rule for object files
%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS)