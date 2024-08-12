#include "genincludes.h"
pair<int,int> set_grid(CellArray& array, string RLE, bool shift=true);
pair<int,int> two_state_RLE_to_grid(CellArray& array, string RLE, bool shift=true);
string two_state_grid_to_RLE(CellArray& in_arr);
pair<int,int> multi_state_RLE_to_grid(CellArray& array, string RLE, bool shift=true);
string multi_state_grid_to_RLE(CellArray& in_arr);
ostream& operator<< (ostream& os, __int128 lrgint);
ostream& operator<< (ostream& os, CellArray& arr);
ostream& operator<< (ostream& os, RuleRep& arr);
ostream& operator<< (ostream& os, RuleRep&& arr);
ostream& operator<< (ostream& os, TransArray& arr);
ostream& operator<< (ostream& os, RulespaceRep& arr);
RulespaceRep rulespace_from_rules(vector<RuleRep>& rules);
RulespaceRep rulespace_from_rules(vector<RuleRep>&& rules);
RuleRep minimum_rule(RulespaceRep& rulespace);
//Can be written for a custom rulespace
unsigned long long int check_frontend(RulespaceRep& rulespace, CellArray& arr);
bool frontend_allowed_for_stable(unsigned long long int frontend);

template <typename T>
void clear_grid(T& array, int x, int y)
{
  for (int i=RANGE;i<x+RANGE;i++)
  {
    std::fill(array[i].begin()+RANGE, array[i].begin()+y+RANGE, 0);
  }
}