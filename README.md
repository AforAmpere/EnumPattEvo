# EnumPattEvo
Enumerates (given time) all stable patterns with set parameters whose evolutions contain a given starting pattern in Isotropic Non-totalistic Cellular Automata

# Purpose and what program to choose
This program is for exploring the Isotropic Non-Totalistic (INT) rulespace in Cellular Automata, by finding rules where a given pattern follows behavior set by the user. When choosing whether of the two programs to compile, EnumPattEvo.cpp or EnumPattEvo2.cpp, the latter should be chosen if the maximum bounding box is relatively small (area < 300 or so), or the density of the pattern in the bounding box is relatively high. If neither of these are the case, the former should be chosen.

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
To use, with executable name as EnumPattEvo, in the terminal, do ./EnumPattEvo (RLE) \[RLE of Final Pattern]. The first argument is the pattern fed into the program to find rules for. The second argument is an end pattern different from the first argument that the program should find an evolution to instead. The second argument is optional, and should not be used if the user desires only spaceships and oscillators with the given pattern in the first argument. Both argument one and two should be in RLE format, but replace characters with numbers preceding them with the extended forms. Thus, '2o3$bo2bo!' becomes 'oo$$$bobbo!'.
