# EnumPattEvo

A program for finding rules in cellular automata where starting patterns evolve in specified ways

## What EPE Does

EPE is designed for taking an initial pattern and finding all examples (given enough time) of whatever constraints are set. For instance, finding all rules in some rulespace where a pattern is an oscillator or a spaceship. EPE aims to complete those searches with the minimal amount of searching to guarantee complete coverage.

## How it Works

EPE does a depth-first search across the rulespace it is given in an attempt to find the rules that meet the search criteria. Each node of the tree corresponds to an increase in one generation from the parent node. The branches from a given node correspond to all of the ways to set the new transitions that have previously been undetermined during the evolution that now must be determined to move forward. In INT, this leads to a branching ratio at a node of 2^t, where t is the number of currently relevant transitions that were previously undetermined. Searching in this manner is vastly more effective in many cases than simply trying to run each rule in a given rulespace. The exceptions to this are instances with very small rulespaces (a few million rules or less), or rulespaces with an overwhelming number of valid results contained in them.

There are a significant amount of branch pruning options that EPE uses to try and narrow down searches to reasonable lengths. For instance, confining the pattern to always be contained within an n-by-m box, or stabilize in some finite, relatively small number of generations are both effective at speeding up searches. The most effective usage of EPE is done through trying to find the most confined search that will also likely give results. Too confined, and many of the results may be pruned off. Too open, and the search may take an extraordinary amount of time (potentially millions of years).

## Getting Started

Once you've cloned the repository, choose a rulespace from one of those seen below, and do `make {rulespace}`. Out of the currently supported rulespaces, HROT is unique in that you can specify range={n} to the make command to specify the range of the neighborhood. Otherwise, it defaults to range 5. Each time you switch rulespaces, you need to recompile, which should only take a few seconds. Alternatively, you can change the output targets for the compilation to different executable names.

If compilation fails, make sure you have make installed, and g++. If there are still issues, please let me know.

## Rulespaces Currently Added

INT, Isotropic Non-Totalistic range 1 2-state rules (non-B0)

INTGen, INT Generations rules

B0INT, B0 2-state INT rules

BWSINT, Black-White 2-state symmetric INT rules

HROT, High Range Outer Totalistic rules

MAP, Asymmetric 2-state range 1 rules (non-B0)

B0MAP, B0 MAP rules

## Searching

Many of the commands available to use are documented in EPE itself, seen by doing `./EPE -h` to list the available ones, or `./EPE -h {list of commands}` to give information on the ones you specify as input.

To perform searching, you need to figure out what you want to look for specifically. There are many possible things to look for, so EPE tres to be very general purpose in terms of rule-finding

### Example

Goal: Find all oscillators in INT that start with obo! and fit within a 5x5 box.

Command: `./EPE -p 'obo!' -b 5 5 -g 10000'`

Output:
```dims: 3,1
1 0 1

time: 0.170162 seconds
Solutions found: 527
Branches traversed: 895964
```

## Custom Pruning and Result Conditions

SECTION TO BE ADDED
