# EnumPattEvo
Enumerates (given time) all stable patterns with set parameters whose evolutions contain a given starting pattern in Isotropic Non-totalistic Cellular Automata

# Purpose and what program to choose
This program is for exploring the Isotropic Non-Totalistic (INT) rulespace in Cellular Automata, by finding rules where a given pattern follows behavior set by the user. This can be used to find spaceships, oscillators, and many other objects with sufficient constraints of the search space. 

# Constants
There are 6 constants at the top of the program in the .cpp files, MAXPOP, MINPOP, MAXX, MAXY, MAXGEN, and MINGEN.
MAXPOP is the maximum population you want the pattern to be able to attain.
MINPOP is the minimum population you want the pattern to be able to attain.
MAXX is the maximum bounding box size the pattern can reach horizontally.
MAXY is the maximum bounding box size the pattern can reach vertically.
MAXGEN is the maximum generation depth to search. For oscillators and spaceships, this is the maximum period.
MINGEN is the minimum reported generation depth a pattern can have.

# Compilation
To compile, do g++ (Chosen .cpp file) -o (Preferred Executable Name) -O3

# Usage 
For now, use EnumPattEvo2.cpp, at least until the first is updated to be somewhat comparable in usefulness. For general usage of the parameters, use -h for help, using -h with an argument gives you more information on it.
