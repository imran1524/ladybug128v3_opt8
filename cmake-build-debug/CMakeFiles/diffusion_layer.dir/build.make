# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/diffusion_layer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/diffusion_layer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/diffusion_layer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/diffusion_layer.dir/flags.make

CMakeFiles/diffusion_layer.dir/main.c.o: CMakeFiles/diffusion_layer.dir/flags.make
CMakeFiles/diffusion_layer.dir/main.c.o: /Users/ik/Library/Mobile\ Documents/com~apple~CloudDocs/Yousuf/MacbookAir/main.c
CMakeFiles/diffusion_layer.dir/main.c.o: CMakeFiles/diffusion_layer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/diffusion_layer.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/diffusion_layer.dir/main.c.o -MF CMakeFiles/diffusion_layer.dir/main.c.o.d -o CMakeFiles/diffusion_layer.dir/main.c.o -c "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/main.c"

CMakeFiles/diffusion_layer.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/diffusion_layer.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/main.c" > CMakeFiles/diffusion_layer.dir/main.c.i

CMakeFiles/diffusion_layer.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/diffusion_layer.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/main.c" -o CMakeFiles/diffusion_layer.dir/main.c.s

CMakeFiles/diffusion_layer.dir/utils.c.o: CMakeFiles/diffusion_layer.dir/flags.make
CMakeFiles/diffusion_layer.dir/utils.c.o: /Users/ik/Library/Mobile\ Documents/com~apple~CloudDocs/Yousuf/MacbookAir/utils.c
CMakeFiles/diffusion_layer.dir/utils.c.o: CMakeFiles/diffusion_layer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/diffusion_layer.dir/utils.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/diffusion_layer.dir/utils.c.o -MF CMakeFiles/diffusion_layer.dir/utils.c.o.d -o CMakeFiles/diffusion_layer.dir/utils.c.o -c "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/utils.c"

CMakeFiles/diffusion_layer.dir/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/diffusion_layer.dir/utils.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/utils.c" > CMakeFiles/diffusion_layer.dir/utils.c.i

CMakeFiles/diffusion_layer.dir/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/diffusion_layer.dir/utils.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/utils.c" -o CMakeFiles/diffusion_layer.dir/utils.c.s

CMakeFiles/diffusion_layer.dir/permutations.c.o: CMakeFiles/diffusion_layer.dir/flags.make
CMakeFiles/diffusion_layer.dir/permutations.c.o: /Users/ik/Library/Mobile\ Documents/com~apple~CloudDocs/Yousuf/MacbookAir/permutations.c
CMakeFiles/diffusion_layer.dir/permutations.c.o: CMakeFiles/diffusion_layer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/diffusion_layer.dir/permutations.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/diffusion_layer.dir/permutations.c.o -MF CMakeFiles/diffusion_layer.dir/permutations.c.o.d -o CMakeFiles/diffusion_layer.dir/permutations.c.o -c "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/permutations.c"

CMakeFiles/diffusion_layer.dir/permutations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/diffusion_layer.dir/permutations.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/permutations.c" > CMakeFiles/diffusion_layer.dir/permutations.c.i

CMakeFiles/diffusion_layer.dir/permutations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/diffusion_layer.dir/permutations.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/permutations.c" -o CMakeFiles/diffusion_layer.dir/permutations.c.s

CMakeFiles/diffusion_layer.dir/round.c.o: CMakeFiles/diffusion_layer.dir/flags.make
CMakeFiles/diffusion_layer.dir/round.c.o: /Users/ik/Library/Mobile\ Documents/com~apple~CloudDocs/Yousuf/MacbookAir/round.c
CMakeFiles/diffusion_layer.dir/round.c.o: CMakeFiles/diffusion_layer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/diffusion_layer.dir/round.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/diffusion_layer.dir/round.c.o -MF CMakeFiles/diffusion_layer.dir/round.c.o.d -o CMakeFiles/diffusion_layer.dir/round.c.o -c "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/round.c"

CMakeFiles/diffusion_layer.dir/round.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/diffusion_layer.dir/round.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/round.c" > CMakeFiles/diffusion_layer.dir/round.c.i

CMakeFiles/diffusion_layer.dir/round.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/diffusion_layer.dir/round.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/round.c" -o CMakeFiles/diffusion_layer.dir/round.c.s

CMakeFiles/diffusion_layer.dir/aead.c.o: CMakeFiles/diffusion_layer.dir/flags.make
CMakeFiles/diffusion_layer.dir/aead.c.o: /Users/ik/Library/Mobile\ Documents/com~apple~CloudDocs/Yousuf/MacbookAir/aead.c
CMakeFiles/diffusion_layer.dir/aead.c.o: CMakeFiles/diffusion_layer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/diffusion_layer.dir/aead.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/diffusion_layer.dir/aead.c.o -MF CMakeFiles/diffusion_layer.dir/aead.c.o.d -o CMakeFiles/diffusion_layer.dir/aead.c.o -c "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/aead.c"

CMakeFiles/diffusion_layer.dir/aead.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/diffusion_layer.dir/aead.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/aead.c" > CMakeFiles/diffusion_layer.dir/aead.c.i

CMakeFiles/diffusion_layer.dir/aead.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/diffusion_layer.dir/aead.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/aead.c" -o CMakeFiles/diffusion_layer.dir/aead.c.s

# Object files for target diffusion_layer
diffusion_layer_OBJECTS = \
"CMakeFiles/diffusion_layer.dir/main.c.o" \
"CMakeFiles/diffusion_layer.dir/utils.c.o" \
"CMakeFiles/diffusion_layer.dir/permutations.c.o" \
"CMakeFiles/diffusion_layer.dir/round.c.o" \
"CMakeFiles/diffusion_layer.dir/aead.c.o"

# External object files for target diffusion_layer
diffusion_layer_EXTERNAL_OBJECTS =

diffusion_layer: CMakeFiles/diffusion_layer.dir/main.c.o
diffusion_layer: CMakeFiles/diffusion_layer.dir/utils.c.o
diffusion_layer: CMakeFiles/diffusion_layer.dir/permutations.c.o
diffusion_layer: CMakeFiles/diffusion_layer.dir/round.c.o
diffusion_layer: CMakeFiles/diffusion_layer.dir/aead.c.o
diffusion_layer: CMakeFiles/diffusion_layer.dir/build.make
diffusion_layer: CMakeFiles/diffusion_layer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable diffusion_layer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/diffusion_layer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/diffusion_layer.dir/build: diffusion_layer
.PHONY : CMakeFiles/diffusion_layer.dir/build

CMakeFiles/diffusion_layer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/diffusion_layer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/diffusion_layer.dir/clean

CMakeFiles/diffusion_layer.dir/depend:
	cd "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir" "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir" "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug" "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug" "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cmake-build-debug/CMakeFiles/diffusion_layer.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/diffusion_layer.dir/depend

