# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build"

# Include any dependencies generated for this target.
include pipes/CMakeFiles/pipes.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include pipes/CMakeFiles/pipes.dir/compiler_depend.make

# Include the progress variables for this target.
include pipes/CMakeFiles/pipes.dir/progress.make

# Include the compile flags for this target's objects.
include pipes/CMakeFiles/pipes.dir/flags.make

pipes/CMakeFiles/pipes.dir/read_pipe.c.o: pipes/CMakeFiles/pipes.dir/flags.make
pipes/CMakeFiles/pipes.dir/read_pipe.c.o: ../pipes/read_pipe.c
pipes/CMakeFiles/pipes.dir/read_pipe.c.o: pipes/CMakeFiles/pipes.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object pipes/CMakeFiles/pipes.dir/read_pipe.c.o"
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT pipes/CMakeFiles/pipes.dir/read_pipe.c.o -MF CMakeFiles/pipes.dir/read_pipe.c.o.d -o CMakeFiles/pipes.dir/read_pipe.c.o -c "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/pipes/read_pipe.c"

pipes/CMakeFiles/pipes.dir/read_pipe.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pipes.dir/read_pipe.c.i"
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/pipes/read_pipe.c" > CMakeFiles/pipes.dir/read_pipe.c.i

pipes/CMakeFiles/pipes.dir/read_pipe.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pipes.dir/read_pipe.c.s"
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/pipes/read_pipe.c" -o CMakeFiles/pipes.dir/read_pipe.c.s

pipes/CMakeFiles/pipes.dir/write_pipe.c.o: pipes/CMakeFiles/pipes.dir/flags.make
pipes/CMakeFiles/pipes.dir/write_pipe.c.o: ../pipes/write_pipe.c
pipes/CMakeFiles/pipes.dir/write_pipe.c.o: pipes/CMakeFiles/pipes.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object pipes/CMakeFiles/pipes.dir/write_pipe.c.o"
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT pipes/CMakeFiles/pipes.dir/write_pipe.c.o -MF CMakeFiles/pipes.dir/write_pipe.c.o.d -o CMakeFiles/pipes.dir/write_pipe.c.o -c "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/pipes/write_pipe.c"

pipes/CMakeFiles/pipes.dir/write_pipe.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pipes.dir/write_pipe.c.i"
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/pipes/write_pipe.c" > CMakeFiles/pipes.dir/write_pipe.c.i

pipes/CMakeFiles/pipes.dir/write_pipe.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pipes.dir/write_pipe.c.s"
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/pipes/write_pipe.c" -o CMakeFiles/pipes.dir/write_pipe.c.s

# Object files for target pipes
pipes_OBJECTS = \
"CMakeFiles/pipes.dir/read_pipe.c.o" \
"CMakeFiles/pipes.dir/write_pipe.c.o"

# External object files for target pipes
pipes_EXTERNAL_OBJECTS =

pipes/libpipes.a: pipes/CMakeFiles/pipes.dir/read_pipe.c.o
pipes/libpipes.a: pipes/CMakeFiles/pipes.dir/write_pipe.c.o
pipes/libpipes.a: pipes/CMakeFiles/pipes.dir/build.make
pipes/libpipes.a: pipes/CMakeFiles/pipes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libpipes.a"
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && $(CMAKE_COMMAND) -P CMakeFiles/pipes.dir/cmake_clean_target.cmake
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pipes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
pipes/CMakeFiles/pipes.dir/build: pipes/libpipes.a
.PHONY : pipes/CMakeFiles/pipes.dir/build

pipes/CMakeFiles/pipes.dir/clean:
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" && $(CMAKE_COMMAND) -P CMakeFiles/pipes.dir/cmake_clean.cmake
.PHONY : pipes/CMakeFiles/pipes.dir/clean

pipes/CMakeFiles/pipes.dir/depend:
	cd "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1" "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/pipes" "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build" "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes" "/mnt/c/Users/krivo/Программирование 1 курс/Практика 12/task1/build/pipes/CMakeFiles/pipes.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : pipes/CMakeFiles/pipes.dir/depend

