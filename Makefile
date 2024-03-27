# Compiler in use
CC=clang

# Compilation flags for both architectures
COMMON_CFLAGS=-Wall -g -I/opt/homebrew/include

# Compilation flags for ARM64 architecture
ARM64_CFLAGS=$(COMMON_CFLAGS) -arch arm64

# Compilation flags for Intel architecture
INTEL_CFLAGS=$(COMMON_CFLAGS)

# Linking flags, example for the math library
LDFLAGS=-L/opt/homebrew/lib -lm

# All source files
SOURCES=main.c permutations.c round.c utils.c aead.c cJSON/cJSON.c

# Convert source files to object files
OBJECTS=$(SOURCES:.c=.o)

# Name of the final executable
EXECUTABLE=diffusion_layer

# Default target builds the executable
all: $(EXECUTABLE)

# Link the program using object files and output an executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# Compile the source files into object files for ARM64 architecture
arm64: CFLAGS=$(ARM64_CFLAGS)
arm64: clean $(EXECUTABLE)

# Compile the source files into object files for Intel architecture
intel: CFLAGS=$(INTEL_CFLAGS)
intel: clean $(EXECUTABLE)

# Rule to compile individual source files into object files
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the workspace
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Target to run AddressSanitizer
asan: CFLAGS += -fsanitize=address
asan: LDFLAGS += -fsanitize=address
asan: clean $(EXECUTABLE)
