# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_SOURCE_DIR = /home/amine102/projects/General

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amine102/projects/General/build

# Include any dependencies generated for this target.
include test/CMakeFiles/pilg_bn_juggler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/pilg_bn_juggler.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/pilg_bn_juggler.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/pilg_bn_juggler.dir/flags.make

test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o: test/CMakeFiles/pilg_bn_juggler.dir/flags.make
test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o: /home/amine102/projects/General/test/bn/bn_juggler.cpp
test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o: test/CMakeFiles/pilg_bn_juggler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/General/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o"
	cd /home/amine102/projects/General/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o -MF CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o.d -o CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o -c /home/amine102/projects/General/test/bn/bn_juggler.cpp

test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.i"
	cd /home/amine102/projects/General/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/General/test/bn/bn_juggler.cpp > CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.i

test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.s"
	cd /home/amine102/projects/General/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/General/test/bn/bn_juggler.cpp -o CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.s

# Object files for target pilg_bn_juggler
pilg_bn_juggler_OBJECTS = \
"CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o"

# External object files for target pilg_bn_juggler
pilg_bn_juggler_EXTERNAL_OBJECTS =

test/pilg_bn_juggler: test/CMakeFiles/pilg_bn_juggler.dir/bn/bn_juggler.cpp.o
test/pilg_bn_juggler: test/CMakeFiles/pilg_bn_juggler.dir/build.make
test/pilg_bn_juggler: libpilg.a
test/pilg_bn_juggler: /usr/lib/x86_64-linux-gnu/libboost_system.a
test/pilg_bn_juggler: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
test/pilg_bn_juggler: /usr/lib/libprobt.so
test/pilg_bn_juggler: test/CMakeFiles/pilg_bn_juggler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/amine102/projects/General/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable pilg_bn_juggler"
	cd /home/amine102/projects/General/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pilg_bn_juggler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/pilg_bn_juggler.dir/build: test/pilg_bn_juggler
.PHONY : test/CMakeFiles/pilg_bn_juggler.dir/build

test/CMakeFiles/pilg_bn_juggler.dir/clean:
	cd /home/amine102/projects/General/build/test && $(CMAKE_COMMAND) -P CMakeFiles/pilg_bn_juggler.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/pilg_bn_juggler.dir/clean

test/CMakeFiles/pilg_bn_juggler.dir/depend:
	cd /home/amine102/projects/General/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amine102/projects/General /home/amine102/projects/General/test /home/amine102/projects/General/build /home/amine102/projects/General/build/test /home/amine102/projects/General/build/test/CMakeFiles/pilg_bn_juggler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/pilg_bn_juggler.dir/depend

