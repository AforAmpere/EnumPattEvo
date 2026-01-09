#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <array>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <random>
#include <queue>
#include <regex>

#define LORGE (1<<23)
#define SMOLL (-(1<<23))
#ifndef ALTERNATING
#define ALTERNATING 1
#endif

using namespace std;

//For cell and transition grids
typedef vector<vector<int>> grid;
//For lists of new transitions
typedef array<int,TRANSCOUNT+1> TransArray;
//For lists of booleans denoting inclusion of transitions
typedef array<bool,TRANSCOUNT> TransSetArray;
//Representations of rules
typedef array<int,TRANSCOUNT> RuleRep;
//Representations of rulespaces
typedef array<vector<int>,TRANSCOUNT> RulespaceRep;

enum print_mode
{
    mode_cell,
    mode_trans
};

extern print_mode cellprintmode;

struct OptionState
{
    static vector<string> startingpatterns;
    static vector<string> targetpatterns;
    static vector<int> xdims;
    static vector<int> ydims;
    static vector<int> gendepths;
    static vector<string> rulespace;
    static vector<int> allow_evolve;
    static vector<int> enforce_move;
    static int minperiod;
    static string filename;
    static int reporting_interval;
    static bool sssssmode;
    static vector<int> prune_minpop;
    static vector<int> prune_maxpop;
    static vector<int> prune_mindispx;
    static vector<int> prune_maxdispx;
    static vector<int> prune_mindispy;
    static vector<int> prune_maxdispy;
    static vector<int> prune_mincomm;
    static vector<int> prune_maxcomm;
    static vector<int> filter_minpop;
    static vector<int> filter_maxpop;
    static vector<int> filter_mindispx;
    static vector<int> filter_maxdispx;
    static vector<int> filter_mindispy;
    static vector<int> filter_maxdispy;
    static vector<int> filter_mincomm;
    static vector<int> filter_maxcomm;
    static bool append;
    static vector<int> nosymm;
    static vector<int> noexp;
    static string sssssfolder;
    static RulespaceRep incomingrulespace;
    static vector<string> slidingmask;
    static vector<string> staticmask;
    static vector<pair<int,int>> staticmaskoff;
    static vector<string> slidingcomp;
    //static vector<pair<int,int>> slidingcomparroff;
    static vector<string> staticcomp;
    static vector<pair<int,int>> staticcompmaskoff;
    //static vector<pair<int,int>> staticcomparroff;
    static vector<string> help;
    static vector<int> startingbranchoffset;
    static string savefile;
    static string loadedargv;
    static long unsigned int randseed;
};

struct CellArray
{
    grid cells;
    grid trans;

    int max_x, max_y;
    int dims_x, dims_y;
    int disp_x, disp_y;
    int pop;
    int symm;
    int patt;
    int period;

    CellArray(int x, int y, int patt);

    int get_cell_trans(int x, int y);

    void generate_trans_array(RulespaceRep& rulespace, TransSetArray& visited);
};

//Things that need to be written for a custom rulespace
RuleRep parse_rule(string rulestring);
string rulestring_from_arr(RuleRep& rule);

//Things that need to be written for a custom symmetry
void find_mask_bounds(CellArray& arr, CellArray& mask, int offx, int offy, int& min_x, int& min_y, int& max_x, int& max_y);
bool compare_cell_arrays(CellArray& arr1, CellArray& arr2);
bool compare_cell_arrays(CellArray& arr1, CellArray& arr2, CellArray& mask, int offx, int offy);
void evolve(RulespaceRep& rulespace, CellArray& prevarray, CellArray& newarray);
int determine_symm(CellArray& arr);
bool symmetry_check(CellArray& arr);
bool possible_symm_transition(int curr, int targ);
string canonized_speed(int dx, int dy, int period);
void canonize_array(CellArray& inarr, CellArray& outarr, int disp_x, int disp_y);
bool check_mask(CellArray& arr, CellArray& mask, int offx, int offy);

queue<pair<int,int>>& linked(int trans, int newstate);

//Special
void load_5s(unordered_map<string,int>& table, string folder);

extern RulespaceRep maxspace;

//extern vector<vector<vector<vector<bool>>>> occupied;