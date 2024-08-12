#include "src/arrayfuncs.h"

char base64table[64]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};

unordered_map<char, int> trans_rev_lookup=
{
    {'A',0},
    {'B',1},
    {'C',2},
    {'D',3},
    {'E',4},
    {'F',5},
    {'G',6},
    {'H',7},
    {'I',8},
    {'J',9},
    {'K',10},
    {'L',11},
    {'M',12},
    {'N',13},
    {'O',14},
    {'P',15},
    {'Q',16},
    {'R',17},
    {'S',18},
    {'T',19},
    {'U',20},
    {'V',21},
    {'W',22},
    {'X',23},
    {'Y',24},
    {'Z',25},
    {'a',26},
    {'b',27},
    {'c',28},
    {'d',29},
    {'e',30},
    {'f',31},
    {'g',32},
    {'h',33},
    {'i',34},
    {'j',35},
    {'k',36},
    {'l',37},
    {'m',38},
    {'n',39},
    {'o',40},
    {'p',41},
    {'q',42},
    {'r',43},
    {'s',44},
    {'t',45},
    {'u',46},
    {'v',47},
    {'w',48},
    {'x',49},
    {'y',50},
    {'z',51},
    {'0',52},
    {'1',53},
    {'2',54},
    {'3',55},
    {'4',56},
    {'5',57},
    {'6',58},
    {'7',59},
    {'8',60},
    {'9',61},
    {'+',62},
    {'/',63}
};

int CellArray::get_cell_trans(int x, int y)
{
    return 
    ((x==0 || y==0) ? 0:(cells[x-1][y-1]<<8))+\
    (y==0 ? 0:(cells[x][y-1])<<7)+\
    (y==0 ? 0:(cells[x+1][y-1])<<6)+\
    (x==0 ? 0:(cells[x-1][y])<<5)+\
    ((cells[x][y])<<4)+\
    ((cells[x+1][y])<<3)+\
    (x==0 ? 0:(cells[x-1][y+1])<<2)+\
    ((cells[x][y+1])<<1)+\
    ((cells[x+1][y+1]));
}

string reg = "MAP[0-9a-zA-Z\\+\\/]{86}";

RuleRep parse_rule(string rule)
{
    if (rule.size()==0 || !regex_match(rule, regex(reg)))
	{
		cout<<"Rule: "<<rule<<" is invalid."<<endl;
		exit(1);
	}
	RuleRep arr_out={0};
    for(int i=0;i<TRANSCOUNT;i++)
    {
        arr_out[i]=(trans_rev_lookup[rule[i/6+3]]>>(5-i%6))&1;
    }
    if(arr_out[0]==1)
    {
        cout<<"B0 is not allowed in this space."<<endl;
        exit(1);
    } 
	return arr_out;
}

string rulestring_from_arr(RuleRep& rule)
{
    string out="MAP";
	for(int i=0;i<TRANSCOUNT/6+1;i++)
	{
        int tmp=0;
		for(int j=0;j<6;j++)
        {
            if((i*6+j)<TRANSCOUNT)
            {
                tmp+=((rule[i*6+j]==1)?1:0)<<(5-j);
            }
        }
        out+=base64table[tmp];
	}
	return out;
}

RulespaceRep maxspace = rulespace_from_rules(vector<RuleRep>({parse_rule("MAPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"),parse_rule("MAPf////////////////////////////////////////////////////////////////////////////////////w")}));

