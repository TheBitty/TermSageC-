# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/leog/Desktop/TermSageC-

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/leog/Desktop/TermSageC-/build

# Include any dependencies generated for this target.
include CMakeFiles/TermSage.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TermSage.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TermSage.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TermSage.dir/flags.make

CMakeFiles/TermSage.dir/codegen:
.PHONY : CMakeFiles/TermSage.dir/codegen

CMakeFiles/TermSage.dir/src/main.cpp.o: CMakeFiles/TermSage.dir/flags.make
CMakeFiles/TermSage.dir/src/main.cpp.o: /Users/leog/Desktop/TermSageC-/src/main.cpp
CMakeFiles/TermSage.dir/src/main.cpp.o: CMakeFiles/TermSage.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/leog/Desktop/TermSageC-/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TermSage.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TermSage.dir/src/main.cpp.o -MF CMakeFiles/TermSage.dir/src/main.cpp.o.d -o CMakeFiles/TermSage.dir/src/main.cpp.o -c /Users/leog/Desktop/TermSageC-/src/main.cpp

CMakeFiles/TermSage.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TermSage.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leog/Desktop/TermSageC-/src/main.cpp > CMakeFiles/TermSage.dir/src/main.cpp.i

CMakeFiles/TermSage.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TermSage.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leog/Desktop/TermSageC-/src/main.cpp -o CMakeFiles/TermSage.dir/src/main.cpp.s

# Object files for target TermSage
TermSage_OBJECTS = \
"CMakeFiles/TermSage.dir/src/main.cpp.o"

# External object files for target TermSage
TermSage_EXTERNAL_OBJECTS =

TermSage: CMakeFiles/TermSage.dir/src/main.cpp.o
TermSage: CMakeFiles/TermSage.dir/build.make
TermSage: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libcurl.tbd
TermSage: /opt/homebrew/lib/libssl.dylib
TermSage: /opt/homebrew/lib/libcrypto.dylib
TermSage: CMakeFiles/TermSage.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/leog/Desktop/TermSageC-/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TermSage"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TermSage.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TermSage.dir/build: TermSage
.PHONY : CMakeFiles/TermSage.dir/build

CMakeFiles/TermSage.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TermSage.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TermSage.dir/clean

CMakeFiles/TermSage.dir/depend:
	cd /Users/leog/Desktop/TermSageC-/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/leog/Desktop/TermSageC- /Users/leog/Desktop/TermSageC- /Users/leog/Desktop/TermSageC-/build /Users/leog/Desktop/TermSageC-/build /Users/leog/Desktop/TermSageC-/build/CMakeFiles/TermSage.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TermSage.dir/depend

