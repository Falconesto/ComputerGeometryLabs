# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/varaastasenko/CLionProjects/CompGraph3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CompGraph3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CompGraph3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CompGraph3.dir/flags.make

CMakeFiles/CompGraph3.dir/main.cpp.o: CMakeFiles/CompGraph3.dir/flags.make
CMakeFiles/CompGraph3.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CompGraph3.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CompGraph3.dir/main.cpp.o -c /Users/varaastasenko/CLionProjects/CompGraph3/main.cpp

CMakeFiles/CompGraph3.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CompGraph3.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/varaastasenko/CLionProjects/CompGraph3/main.cpp > CMakeFiles/CompGraph3.dir/main.cpp.i

CMakeFiles/CompGraph3.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CompGraph3.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/varaastasenko/CLionProjects/CompGraph3/main.cpp -o CMakeFiles/CompGraph3.dir/main.cpp.s

CMakeFiles/CompGraph3.dir/picture.cpp.o: CMakeFiles/CompGraph3.dir/flags.make
CMakeFiles/CompGraph3.dir/picture.cpp.o: ../picture.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CompGraph3.dir/picture.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CompGraph3.dir/picture.cpp.o -c /Users/varaastasenko/CLionProjects/CompGraph3/picture.cpp

CMakeFiles/CompGraph3.dir/picture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CompGraph3.dir/picture.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/varaastasenko/CLionProjects/CompGraph3/picture.cpp > CMakeFiles/CompGraph3.dir/picture.cpp.i

CMakeFiles/CompGraph3.dir/picture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CompGraph3.dir/picture.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/varaastasenko/CLionProjects/CompGraph3/picture.cpp -o CMakeFiles/CompGraph3.dir/picture.cpp.s

# Object files for target CompGraph3
CompGraph3_OBJECTS = \
"CMakeFiles/CompGraph3.dir/main.cpp.o" \
"CMakeFiles/CompGraph3.dir/picture.cpp.o"

# External object files for target CompGraph3
CompGraph3_EXTERNAL_OBJECTS =

CompGraph3: CMakeFiles/CompGraph3.dir/main.cpp.o
CompGraph3: CMakeFiles/CompGraph3.dir/picture.cpp.o
CompGraph3: CMakeFiles/CompGraph3.dir/build.make
CompGraph3: CMakeFiles/CompGraph3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable CompGraph3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CompGraph3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CompGraph3.dir/build: CompGraph3

.PHONY : CMakeFiles/CompGraph3.dir/build

CMakeFiles/CompGraph3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CompGraph3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CompGraph3.dir/clean

CMakeFiles/CompGraph3.dir/depend:
	cd /Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/varaastasenko/CLionProjects/CompGraph3 /Users/varaastasenko/CLionProjects/CompGraph3 /Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug /Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug /Users/varaastasenko/CLionProjects/CompGraph3/cmake-build-debug/CMakeFiles/CompGraph3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CompGraph3.dir/depend

