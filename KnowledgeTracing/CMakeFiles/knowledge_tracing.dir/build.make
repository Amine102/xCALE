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
CMAKE_SOURCE_DIR = /home/amine102/projects/KnowledgeTracing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amine102/projects/KnowledgeTracing

# Include any dependencies generated for this target.
include CMakeFiles/knowledge_tracing.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/knowledge_tracing.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/knowledge_tracing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/knowledge_tracing.dir/flags.make

CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o: usecases/main.cpp
CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/main.cpp

CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/main.cpp > CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/main.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.s

CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o: core/src/genericBKTModel.cpp
CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o -MF CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o -c /home/amine102/projects/KnowledgeTracing/core/src/genericBKTModel.cpp

CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/core/src/genericBKTModel.cpp > CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.i

CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/core/src/genericBKTModel.cpp -o CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.s

CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o: core/src/student.cpp
CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o -MF CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o -c /home/amine102/projects/KnowledgeTracing/core/src/student.cpp

CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/core/src/student.cpp > CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.i

CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/core/src/student.cpp -o CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.s

CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o: core/src/exercise.cpp
CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o -MF CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o -c /home/amine102/projects/KnowledgeTracing/core/src/exercise.cpp

CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/core/src/exercise.cpp > CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.i

CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/core/src/exercise.cpp -o CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.s

CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o: core/src/tools.cpp
CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o -MF CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o -c /home/amine102/projects/KnowledgeTracing/core/src/tools.cpp

CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/core/src/tools.cpp > CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.i

CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/core/src/tools.cpp -o CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.s

CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o: core/src/debugtools.cpp
CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o -MF CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o -c /home/amine102/projects/KnowledgeTracing/core/src/debugtools.cpp

CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/core/src/debugtools.cpp > CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.i

CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/core/src/debugtools.cpp -o CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.s

CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o: /home/amine102/projects/jsoncpp/dist/jsoncpp.cpp
CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o -MF CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o -c /home/amine102/projects/jsoncpp/dist/jsoncpp.cpp

CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/jsoncpp/dist/jsoncpp.cpp > CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.i

CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/jsoncpp/dist/jsoncpp.cpp -o CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o: usecases/src/progressiveBKT.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/progressiveBKT.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/progressiveBKT.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/progressiveBKT.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o: usecases/src/studentTest.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/studentTest.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/studentTest.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/studentTest.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o: usecases/src/scalabilityTest.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/scalabilityTest.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/scalabilityTest.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/scalabilityTest.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o: usecases/src/impacted.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/impacted.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/impacted.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/impacted.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o: usecases/src/serialize_deserialize.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/serialize_deserialize.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/serialize_deserialize.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/serialize_deserialize.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o: usecases/src/xcaleAUTO.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/xcaleAUTO.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/xcaleAUTO.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/xcaleAUTO.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o: usecases/src/xcale.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/xcale.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/xcale.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/xcale.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.s

CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o: CMakeFiles/knowledge_tracing.dir/flags.make
CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o: usecases/src/prerequisite.cpp
CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o: CMakeFiles/knowledge_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o -MF CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o.d -o CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o -c /home/amine102/projects/KnowledgeTracing/usecases/src/prerequisite.cpp

CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine102/projects/KnowledgeTracing/usecases/src/prerequisite.cpp > CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.i

CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine102/projects/KnowledgeTracing/usecases/src/prerequisite.cpp -o CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.s

# Object files for target knowledge_tracing
knowledge_tracing_OBJECTS = \
"CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o" \
"CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o"

# External object files for target knowledge_tracing
knowledge_tracing_EXTERNAL_OBJECTS =

knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/main.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/core/src/genericBKTModel.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/core/src/student.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/core/src/exercise.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/core/src/tools.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/core/src/debugtools.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/home/amine102/projects/jsoncpp/dist/jsoncpp.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/progressiveBKT.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/studentTest.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/scalabilityTest.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/impacted.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/serialize_deserialize.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/xcaleAUTO.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/xcale.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/usecases/src/prerequisite.cpp.o
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/build.make
knowledge_tracing: /usr/lib/libpilg.a
knowledge_tracing: /usr/lib/x86_64-linux-gnu/libboost_system.so
knowledge_tracing: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
knowledge_tracing: /usr/lib/libprobt.so
knowledge_tracing: CMakeFiles/knowledge_tracing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/amine102/projects/KnowledgeTracing/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable knowledge_tracing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/knowledge_tracing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/knowledge_tracing.dir/build: knowledge_tracing
.PHONY : CMakeFiles/knowledge_tracing.dir/build

CMakeFiles/knowledge_tracing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/knowledge_tracing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/knowledge_tracing.dir/clean

CMakeFiles/knowledge_tracing.dir/depend:
	cd /home/amine102/projects/KnowledgeTracing && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amine102/projects/KnowledgeTracing /home/amine102/projects/KnowledgeTracing /home/amine102/projects/KnowledgeTracing /home/amine102/projects/KnowledgeTracing /home/amine102/projects/KnowledgeTracing/CMakeFiles/knowledge_tracing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/knowledge_tracing.dir/depend

