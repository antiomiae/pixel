# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.8.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.8.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build/cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build

# Include any dependencies generated for this target.
include CMakeFiles/visualinfo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/visualinfo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/visualinfo.dir/flags.make

CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o: CMakeFiles/visualinfo.dir/flags.make
CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o: /Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kevinward/projects/gl/src/lib/glew-2.1.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o   -c /Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c

CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c > CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.i

CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c -o CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.s

CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.requires:

.PHONY : CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.requires

CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.provides: CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.requires
	$(MAKE) -f CMakeFiles/visualinfo.dir/build.make CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.provides.build
.PHONY : CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.provides

CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.provides.build: CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o


# Object files for target visualinfo
visualinfo_OBJECTS = \
"CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o"

# External object files for target visualinfo
visualinfo_EXTERNAL_OBJECTS =

bin/visualinfo: CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o
bin/visualinfo: CMakeFiles/visualinfo.dir/build.make
bin/visualinfo: lib/libGLEW.2.1.0.dylib
bin/visualinfo: CMakeFiles/visualinfo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kevinward/projects/gl/src/lib/glew-2.1.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/visualinfo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/visualinfo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/visualinfo.dir/build: bin/visualinfo

.PHONY : CMakeFiles/visualinfo.dir/build

CMakeFiles/visualinfo.dir/requires: CMakeFiles/visualinfo.dir/Users/kevinward/projects/gl/src/lib/glew-2.1.0/src/visualinfo.c.o.requires

.PHONY : CMakeFiles/visualinfo.dir/requires

CMakeFiles/visualinfo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/visualinfo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/visualinfo.dir/clean

CMakeFiles/visualinfo.dir/depend:
	cd /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build/cmake /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build/cmake /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build /Users/kevinward/projects/gl/src/lib/glew-2.1.0/build/CMakeFiles/visualinfo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/visualinfo.dir/depend

