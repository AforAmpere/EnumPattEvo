# EnumPattEvo
Enumerates (given time) all stable patterns with set parameters whose evolutions contain a given starting pattern in Isotropic Non-totalistic Cellular Automata

# Using (will be expanded):
Compile with g++ EnumPattEvo.c -o (Your preferred file name here) -O3
In the below examples, I will use EnumPattEvo as the executable name.

To run, do ./EnumPattEvo 'RLE' OutputFile. The RLE must be in quotes, and at the moment, all RLEs with numbers must be converted to those without.
For example, 'o3bo2$2b2o!' becomes 'obbbo$$bboo!'. The output file is optional, if none is specified, it will be sent to 'Output - EnumPattEvo.txt'.

The maximum size the pattern can attain, the maximum population it can get to, and the maximum generation depth to search to are specified at the top of the source file.
A good test run is to set MAXPOP to 100, MINPOP to 1, MAXX and MAXY at 5, and MAXGEN to 1000, then run 

./EnumPattEvo 'obo!'.

It should report 527 patterns found in somewhere around 3-4 seconds if your computer is good.

(MORE COMING SOON)
