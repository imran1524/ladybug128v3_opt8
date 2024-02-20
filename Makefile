CC=clang # Compiler in use
CFLAGS=-Wall -g -arch arm64 -I/opt/homebrew/include # Compilation flags
LDFLAGS=-L/opt/homebrew/lib -lm # Linking flags, example for the math library
SOURCES=main.c diffusion_layer.c confusion_layer.c utils.c# All source files
OBJECTS=$(SOURCES:.c=.o) # Convert source files to object files
EXECUTABLE=diffusion_layer # Name of the final executable

# Default target builds the executable
all: $(EXECUTABLE)

# Link the program using object files and output an executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

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

