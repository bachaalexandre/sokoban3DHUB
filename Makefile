##
## EPITECH PROJECT, 2025
## sokoban3D
## File description:
## Makefile
##

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = sokoban3d
BUILD_DIR = build
SRC_DIR = src

RAYLIB_DIR = raylib
RAYLIB_INCLUDE = $(RAYLIB_DIR)/src
RAYLIB_LIB = $(RAYLIB_DIR)/src/libraylib.a

INCLUDES = -I$(RAYLIB_INCLUDE) -I$(SRC_DIR)
LIBS = -lGL -lm -lpthread -ldl -lrt -lX11

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIBS += -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)
    LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

SOURCES = $(shell find $(SRC_DIR) -name "*.cpp")
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

SUBDIRS = $(sort $(dir $(OBJECTS)))

all: $(TARGET)

$(TARGET): $(RAYLIB_LIB) $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) $(RAYLIB_LIB) $(LIBS) -o $(TARGET)
	@echo "Build complete!"

$(RAYLIB_LIB):
	@echo "Building Raylib..."
	@if [ ! -d "$(RAYLIB_DIR)" ]; then \
		echo "Error: Raylib directory not found at $(RAYLIB_DIR)"; \
		echo "Please make sure Raylib is installed in the raylib/ directory"; \
		exit 1; \
	fi
	$(MAKE) -C $(RAYLIB_DIR)/src

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(SUBDIRS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(SUBDIRS):
	@mkdir -p $@

clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

clean-all: clean
	@echo "Cleaning Raylib..."
	$(MAKE) -C $(RAYLIB_DIR)/src clean

re: clean all

install-deps:
	@echo "Installing dependencies..."
	sudo apt update
	sudo apt install build-essential git cmake
	sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev

help:
	@echo "Available targets:"
	@echo "  all         - Build the project (default)"
	@echo "  clean       - Remove build files"
	@echo "  clean-all   - Remove build files and clean Raylib"
	@echo "  rebuild     - Clean and build"
	@echo "  install-deps- Install system dependencies (Ubuntu/Debian)"
	@echo "  help        - Show this help"

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

release: CXXFLAGS += -O3 -DNDEBUG
release: $(TARGET)

.PHONY: all clean clean-all rebuild install-deps help debug release

print-%:
	@echo $* = $($*)