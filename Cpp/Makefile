# flags #
CXXFLAGS = -Wall -O0 -g3 -std=c++17
INCLUDES = -I include/ -I /usr/local/include

LIBS = -lpthread -lconfig++
LDFLAGS = $(LIBS)

CXX ?= g++

# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# executable #
BIN_NAME = main.exe

# extensions #
SRC_EXT = cpp

# Find all source files in the source directory, sorted by most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Criando pastas"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deletando atalho $(BIN_NAME)"
	@$(RM) $(BIN_NAME)
	@echo "Deletando pastas e arquivos"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Criando atalho: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linkando: $@"
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compilando: $< -> $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@

.PHONY: remake
remake:
	@$(MAKE) clean
	@$(MAKE)

.PHONY: run
run: remake
	@clear
	@./$(BIN_NAME)

.PHONY: val
val: remake
	@clear
	valgrind --leak-check=yes --track-origins=yes  ./$(BIN_NAME)

.PHONY: val-full
val-full: remake
	@clear
	valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_NAME)
