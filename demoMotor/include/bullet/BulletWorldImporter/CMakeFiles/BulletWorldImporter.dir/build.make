# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jordi/irrlicht/bullet3-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jordi/irrlicht/bullet3-master

# Include any dependencies generated for this target.
include Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend.make

# Include the progress variables for this target.
include Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/progress.make

# Include the compile flags for this target's objects.
include Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o: Extras/Serialize/BulletWorldImporter/btMultiBodyWorldImporter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jordi/irrlicht/bullet3-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o -c /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btMultiBodyWorldImporter.cpp

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.i"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btMultiBodyWorldImporter.cpp > CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.i

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.s"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btMultiBodyWorldImporter.cpp -o CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.s

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.requires:

.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.requires

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.provides: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.requires
	$(MAKE) -f Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.provides.build
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.provides

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.provides.build: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o


Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o: Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jordi/irrlicht/bullet3-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o -c /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp > CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp -o CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires:

.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires
	$(MAKE) -f Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides.build
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides.build: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o


Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o: Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jordi/irrlicht/bullet3-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o -c /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp > CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp -o CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires:

.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires
	$(MAKE) -f Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides.build
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides.build: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o


# Object files for target BulletWorldImporter
BulletWorldImporter_OBJECTS = \
"CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o" \
"CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o" \
"CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o"

# External object files for target BulletWorldImporter
BulletWorldImporter_EXTERNAL_OBJECTS =

Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: src/BulletDynamics/libBulletDynamics.so.2.88
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: src/BulletCollision/libBulletCollision.so.2.88
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: Extras/Serialize/BulletFileLoader/libBulletFileLoader.so.2.88
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: src/LinearMath/libLinearMath.so.2.88
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jordi/irrlicht/bullet3-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library libBulletWorldImporter.so"
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BulletWorldImporter.dir/link.txt --verbose=$(VERBOSE)
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -E cmake_symlink_library libBulletWorldImporter.so.2.88 libBulletWorldImporter.so.2.88 libBulletWorldImporter.so

Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so: Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so.2.88
	@$(CMAKE_COMMAND) -E touch_nocreate Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so

# Rule to build all files generated by this target.
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build: Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.so

.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/requires: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btMultiBodyWorldImporter.o.requires
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/requires: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/requires: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires

.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/requires

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/clean:
	cd /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -P CMakeFiles/BulletWorldImporter.dir/cmake_clean.cmake
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/clean

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend:
	cd /home/jordi/irrlicht/bullet3-master && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jordi/irrlicht/bullet3-master /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter /home/jordi/irrlicht/bullet3-master /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter /home/jordi/irrlicht/bullet3-master/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend

