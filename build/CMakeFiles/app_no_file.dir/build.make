# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/lukasz/HardwareRenderer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lukasz/HardwareRenderer/build

# Include any dependencies generated for this target.
include CMakeFiles/app_no_file.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/app_no_file.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/app_no_file.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/app_no_file.dir/flags.make

CMakeFiles/app_no_file.dir/main.cpp.o: CMakeFiles/app_no_file.dir/flags.make
CMakeFiles/app_no_file.dir/main.cpp.o: /home/lukasz/HardwareRenderer/main.cpp
CMakeFiles/app_no_file.dir/main.cpp.o: CMakeFiles/app_no_file.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukasz/HardwareRenderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/app_no_file.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app_no_file.dir/main.cpp.o -MF CMakeFiles/app_no_file.dir/main.cpp.o.d -o CMakeFiles/app_no_file.dir/main.cpp.o -c /home/lukasz/HardwareRenderer/main.cpp

CMakeFiles/app_no_file.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/app_no_file.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukasz/HardwareRenderer/main.cpp > CMakeFiles/app_no_file.dir/main.cpp.i

CMakeFiles/app_no_file.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/app_no_file.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukasz/HardwareRenderer/main.cpp -o CMakeFiles/app_no_file.dir/main.cpp.s

CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o: CMakeFiles/app_no_file.dir/flags.make
CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o: /home/lukasz/HardwareRenderer/rendering_api/rendering_pipeline/gpu_context.cpp
CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o: CMakeFiles/app_no_file.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukasz/HardwareRenderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o -MF CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o.d -o CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o -c /home/lukasz/HardwareRenderer/rendering_api/rendering_pipeline/gpu_context.cpp

CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukasz/HardwareRenderer/rendering_api/rendering_pipeline/gpu_context.cpp > CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.i

CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukasz/HardwareRenderer/rendering_api/rendering_pipeline/gpu_context.cpp -o CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.s

CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o: CMakeFiles/app_no_file.dir/flags.make
CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o: /home/lukasz/HardwareRenderer/rendering_api/util/log/log.cpp
CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o: CMakeFiles/app_no_file.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukasz/HardwareRenderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o -MF CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o.d -o CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o -c /home/lukasz/HardwareRenderer/rendering_api/util/log/log.cpp

CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukasz/HardwareRenderer/rendering_api/util/log/log.cpp > CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.i

CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukasz/HardwareRenderer/rendering_api/util/log/log.cpp -o CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.s

# Object files for target app_no_file
app_no_file_OBJECTS = \
"CMakeFiles/app_no_file.dir/main.cpp.o" \
"CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o" \
"CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o"

# External object files for target app_no_file
app_no_file_EXTERNAL_OBJECTS =

app_no_file: CMakeFiles/app_no_file.dir/main.cpp.o
app_no_file: CMakeFiles/app_no_file.dir/rendering_api/rendering_pipeline/gpu_context.cpp.o
app_no_file: CMakeFiles/app_no_file.dir/rendering_api/util/log/log.cpp.o
app_no_file: CMakeFiles/app_no_file.dir/build.make
app_no_file: /usr/lib/x86_64-linux-gnu/libOpenCL.so
app_no_file: /usr/lib/x86_64-linux-gnu/libSDL2.so
app_no_file: /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so
app_no_file: CMakeFiles/app_no_file.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/lukasz/HardwareRenderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable app_no_file"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app_no_file.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/app_no_file.dir/build: app_no_file
.PHONY : CMakeFiles/app_no_file.dir/build

CMakeFiles/app_no_file.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/app_no_file.dir/cmake_clean.cmake
.PHONY : CMakeFiles/app_no_file.dir/clean

CMakeFiles/app_no_file.dir/depend:
	cd /home/lukasz/HardwareRenderer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lukasz/HardwareRenderer /home/lukasz/HardwareRenderer /home/lukasz/HardwareRenderer/build /home/lukasz/HardwareRenderer/build /home/lukasz/HardwareRenderer/build/CMakeFiles/app_no_file.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/app_no_file.dir/depend

