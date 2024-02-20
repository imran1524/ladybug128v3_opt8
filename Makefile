CC=clang # Compiler in use
CFLAGS=-Wall -g -arch arm64 -I/opt/homebrew/include# Compilation flags: -Wall for all warning, -g for debugging information, specify architecture
LDFLAGS=-L/opt/homebrew/lib -lm # Example for the math library
SOURCES=main.c diffusion_layer.c# List of all source files. Add more files separated by space
OBJECTS=$(SOURCES:.c=.o) # Object files corresponding to source files
EXECUTABLE=diffusion_layer

# Default target
all: $(EXECUTABLE)

# Link the program
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Compile the source files into object files
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the workspace
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Target to run AddressSanitizer
asan: CFLAGS += -fsanitize=address
asan: LDFLAGS += -fsanitize=address
asan: clean $(EXECUTABLE)

