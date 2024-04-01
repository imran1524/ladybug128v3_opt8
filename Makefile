# Detect OS and Compiler
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CC ?= gcc
    LIB_PATH = /usr/lib
    INCLUDE_PATH = /usr/include
    RM := rm -f
    RMDIR := rm -rf
endif

ifeq ($(UNAME_S),Darwin)
    CC ?= clang
    LIB_PATH = /opt/homebrew/lib
    INCLUDE_PATH = /opt/homebrew/include
    RM := rm -f
    RMDIR := rm -rf
endif
ifeq ($(OS),Windows_NT)
    CC := gcc
    INCLUDE_PATH := include
    RM := cmd /c del /Q
    RMDIR := cmd /c rmdir /S /Q
endif

# Default compilation flags
CFLAGS := -Wall -g -I./$(INCLUDE_PATH)

# Linking flags
LDFLAGS := -L$(LIB_PATH) -lm

# Architecture-specific flags
ARM64_CFLAGS := $(CFLAGS) -arch arm64
INTEL_CFLAGS := $(CFLAGS)

# Source and object files
SOURCES := main.c permutations.c round.c utils.c encrypt.c cJSON/cJSON.c
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE := diffusion_layer

# Default target
all: $(EXECUTABLE)

# Executable linking
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
# Object files compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
# Clean workspace
clean:
ifeq ($(OS),Windows_NT)
	@$(foreach file,$(OBJECTS) $(EXECUTABLE).exe, $(RM), $(subst /,\,$(file)) del /Q $(subst /,\,$(file)) &)
else
	$(RM) $(OBJECTS) $(EXECUTABLE)
endif

# AddressSanitizer target
asan: CFLAGS += -fsanitize=address
asan: LDFLAGS += -fsanitize=address
asan: clean $(EXECUTABLE)

# Architecture-specific targets
arm64: CFLAGS := $(ARM64_CFLAGS)
arm64: clean $(EXECUTABLE)

intel: CFLAGS := $(INTEL_CFLAGS)
intel: clean $(EXECUTABLE)