# Assignment 3

Milestone 0: 
CMake - Sep 15
Unit Testing filled out 
Due: Friday, September 20, 2019 11:59 PM

Milestone 1: PCB file loading and First Come, First Served 
Due: Friday, September 27, 2019 11:59 PM

Milestone 2: Priority scheduling, Round Robin, and analysis of algorithms 
Due: Friday, October 4, 2019 11:59 PM

Note: 
1. After you run assignment3_test, the PCBs.bin will be created in the ./test directory.
   so in the ./build directory, you can run the analysis program like "./analysis ../test/PCBs.bin RR 4"; or you can simply copy and paste the PCBs.bin to the ./build directory, and then run "./analysis PCBs.bin RR 4".
     
2. You can manually copy the time analysis from console and paste it to this readme file, but directly output from your program is strongly recommended.     
    
    ---
    Add your scheduling algorithm analysis below this line in a readable format. 


# CMAKE

- include_directories

`include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])`

Add the given directories to those the compiler uses to search for include files. Relative paths are interpreted as relative to the current source directory.

PROJECT_SOURCE_DIR : contains the full path to the root of your
project source directory, i.e. to the nearest directory where
CMakeLists.txt contains the PROJECT() command

- add_library

`add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] source1 [source2 ...])`

Adds a library target called "name" to be built from the source files listed in the command invocation.



