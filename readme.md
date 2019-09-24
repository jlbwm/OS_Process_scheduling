# Assignment 3

Milestone 0: 
CMake - Sep 15
Unit Testing filled out Sep 19
Due: Friday, September 20, 2019 11:59 PM

Milestone 1: 
PCB file loading Sep 24
First Come, First Served  Sep 24
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


# OPEN & FOPEN

- `FILE *fopen(const char *path, const char *mode)`
- `int open(const char *pathname, int flags, mode_t mode)`

mode: 
- S_IRUSR: read by owner
- S_IWUSR: write by owner
- S_IRGRP: read by group
- S_IWGRP: write by group

flags:
- O_CREAT: If pathname does not exist, create it as a regular file
- O_TRUNC: If the file already exists and is a regular file and the
              access mode allows writing (i.e., is O_RDWR or O_WRONLY) it
              will be truncated to length 0.  If the file is a FIFO or ter‐
              minal device file, the O_TRUNC flag is ignored.  Otherwise,
              the effect of O_TRUNC is unspecified.
- O_WRONLY: Opening the file write-only.

</br>

    open() is a low-level os call. fdopen() converts an os-level file descriptor to the higher-level FILE-abstraction of the C language. fopen() calls open() in the background and gives you a FILE-pointer directly.

1. fopen provides with buffering IO that may turn out to be a lot faster than what you're doing with open (fopen faster).

2. fopen does line ending translation if the file is not opened in binary mode, which can be very helpful if your program is ever ported to a non-Unix environment (though the world appears to be converging on LF-only (except IETF text-based networking protocols like SMTP and HTTP and such)).

3. FILE * gives you the ability to use fscanf and other stdio functions.

4. Your code may someday need to be ported to some other platform that only supports ANSI C and does not support the open function. 