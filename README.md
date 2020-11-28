# EnumPattEvo
Enumerates (given time) all stable patterns with set parameters whose evolutions contain a given starting pattern in Isotropic Non-totalistic Cellular Automata

# Using (will be expanded):
Compile and use EnumPattEvo2.cpp if the patterns you are working with fit 
in an area somewhere less than 300 or so all the time. For large ones, use EnumPattEvo.cpp
Compile with g++ EnumPattEvo.cpp -o (Your preferred file name here) -O3
In the below examples, I will use EnumPattEvo as the executable name.

To run, do ./EnumPattEvo 'RLE' 'End Pattern' The end pattern is optional, do not specify any if you just want to find oscs or ships with the first pattern. Both must be formatted in RLE format, but the numbers are removed and replaced with the amount indicated. For example, '2o3$bo2bo!' becomes 'oo$$$bobbo!'.

The maximum size the pattern can attain, the maximum population it can get to, and the maximum generation depth to search to are specified at the top of the source file.
The source must be recompiled for any change to these parameters. A good test run is to set MAXPOP to 100, MINPOP to 1, MAXX and MAXY at 5, and MAXGEN to 1000, recompile, then run: 

./EnumPattEvo 'obo!'.

It should report 527 patterns found in somewhere around 0.5-0.6 seconds if your computer is good.

(MORE COMING SOON)
