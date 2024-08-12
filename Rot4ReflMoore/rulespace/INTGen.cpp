#include "src/arrayfuncs.h"

const int trans_lookup[512] = {
0, 1, 2, 6, 1, 3, 6, 13, 2, 6, 4, 12, 5, 14, 17, 22,
51, 52, 53, 57, 52, 54, 57, 64, 53, 57, 55, 63, 56, 65, 68, 73,
2, 5, 4, 17, 6, 14, 12, 22, 7, 18, 10, 28, 18, 23, 28, 36,
53, 56, 55, 68, 57, 65, 63, 73, 58, 69, 61, 79, 69, 74, 79, 87, 
1, 3, 5, 14, 8, 9, 16, 24, 6, 13, 17, 22, 16, 24, 30, 35, 
52, 54, 56, 65, 59, 60, 67, 75, 57, 64, 68, 73, 67, 75, 81, 86, 
5, 15, 11, 21, 16, 25, 26, 40, 18, 29, 27, 37, 31, 41, 39, 45, 
56, 66, 62, 72, 67, 76, 77, 91, 69, 80, 78, 88, 82, 92, 90, 96, 
2, 5, 7, 18, 5, 15, 18, 29, 4, 17, 10, 28, 11, 21, 27, 37, 
53, 56, 58, 69, 56, 66, 69, 80, 55, 68, 61, 79, 62, 72, 78, 88, 
4, 11, 10, 27, 17, 21, 28, 37, 10, 27, 20, 32, 27, 38, 32, 42, 
55, 62, 61, 78, 68, 72, 79, 88, 61, 78, 71, 83, 78, 89, 83, 93, 
6, 14, 18, 23, 16, 25, 31, 41, 12, 22, 28, 36, 26, 40, 39, 45, 
57, 65, 69, 74, 67, 76, 82, 92, 63, 73, 79, 87, 77, 91, 90, 96, 
17, 21, 27, 38, 30, 34, 39, 44, 28, 37, 32, 42, 39, 44, 47, 48, 
68, 72, 78, 89, 81, 85, 90, 95, 79, 88, 83, 93, 90, 95, 98, 99, 
1, 8, 5, 16, 3, 9, 14, 24, 5, 16, 11, 26, 15, 25, 21, 40, 
52, 59, 56, 67, 54, 60, 65, 75, 56, 67, 62, 77, 66, 76, 72, 91, 
6, 16, 17, 30, 13, 24, 22, 35, 18, 31, 27, 39, 29, 41, 37, 45, 
57, 67, 68, 81, 64, 75, 73, 86, 69, 82, 78, 90, 80, 92, 88, 96, 
3, 9, 15, 25, 9, 19, 25, 33, 14, 24, 21, 40, 25, 33, 34, 43, 
54, 60, 66, 76, 60, 70, 76, 84, 65, 75, 72, 91, 76, 84, 85, 94, 
14, 25, 21, 34, 24, 33, 40, 43, 23, 41, 38, 44, 41, 46, 44, 49, 
65, 76, 72, 85, 75, 84, 91, 94, 74, 92, 89, 95, 92, 97, 95, 100, 
6, 16, 18, 31, 14, 25, 23, 41, 17, 30, 27, 39, 21, 34, 38, 44, 
57, 67, 69, 82, 65, 76, 74, 92, 68, 81, 78, 90, 72, 85, 89, 95, 
12, 26, 28, 39, 22, 40, 36, 45, 28, 39, 32, 47, 37, 44, 42, 48, 
63, 77, 79, 90, 73, 91, 87, 96, 79, 90, 83, 98, 88, 95, 93, 99, 
13, 24, 29, 41, 24, 33, 41, 46, 22, 35, 37, 45, 40, 43, 44, 49, 
64, 75, 80, 92, 75, 84, 92, 97, 73, 86, 88, 96, 91, 94, 95, 100, 
22, 40, 37, 44, 35, 43, 45, 49, 36, 45, 42, 48, 45, 49, 48, 50, 
73, 91, 88, 95, 86, 94, 96, 100, 87, 96, 93, 99, 96, 100, 99, 101
};

const char *trans_name_lookup[51]={
"0c",
"1c","1e",
"2c","2e","2k","2a","2i","2n",
"3c","3e","3k","3a","3i","3n","3y","3q","3j","3r",
"4c","4e","4k","4a","4i","4n","4y","4q","4j","4r","4t","4w","4z",
"5c","5e","5k","5a","5i","5n","5y","5q","5j","5r",
"6c","6e","6k","6a","6i","6n",
"7c","7e",
"8c"
};

unordered_map<char, int> trans_rev_lookup=
{
	{'c',0},
	{'e',1},
	{'k',2},
	{'a',3},
	{'i',4},
	{'n',5},
	{'y',6},
	{'q',7},
	{'j',8},
	{'r',9},
	{'t',10},
	{'w',11},
	{'z',12},
};
const int OT_Pos[10]={0,1,3,9,19,32,42,48,50,51};

int CellArray::get_cell_trans(int x, int y)
{
    if(cells[x][y]<2)
    {
    return trans_lookup[((x==0 || y==0) ? 0:(cells[x-1][y-1]==1))+\
    (y==0 ? 0:(cells[x][y-1]==1)<<1)+\
    (y==0 ? 0:(cells[x+1][y-1]==1)<<2)+\
    (x==0 ? 0:(cells[x-1][y]==1)<<3)+\
    ((cells[x][y]==1)<<4)+\
    ((cells[x+1][y]==1)<<5)+\
    (x==0 ? 0:(cells[x-1][y+1]==1)<<6)+\
    ((cells[x][y+1]==1)<<7)+\
    ((cells[x+1][y+1]==1)<<8)];
    }
    else
    {
        return 100+cells[x][y];
    }
}

RuleRep parse_rule(string rule)
{
    RuleRep arr_out={0};
    for(int i=51;i<102;i++) arr_out[i]=2;
    for(int i=102;i<TRANSCOUNT;i++) arr_out[i]=0;
	int BS=-1;
	int minusflag=0;
	char num = '#';
	if (rule.size()==0)
	{
		cout<<"Rule: "<<rule<<" invalid."<<endl;
		return arr_out;
	}
	BS=1;
	for(int i=0;i<rule.size();i++)
	{
		if(rule[i]=='/')
		{
			BS--;
		}
        if(BS==-1)
        {
            int states = stoi(rule.substr(i+1,string::npos));
            for(int j=102;j<TRANSCOUNT;j++)
            {
                if(j<=102+states-4)
                    arr_out[j]=j-99;
                else
                    arr_out[j]=0;
            }
            break;
        }
		if(rule[i]>='0'&&rule[i]<='9')
		{
			num=rule[i];
			minusflag=0;
			if(i==rule.size()-1||(rule[i+1]>='0'&&rule[i+1]<='9')||rule[i+1]=='-'||rule[i+1]=='/')
			{
				if(rule[i+1]=='-') minusflag=1;
				for(int j=OT_Pos[rule[i]-'0'];j<OT_Pos[rule[i]-'0'+1];j++)
				{
					arr_out[BS*51+j]=1;
				}
			}
			else
			{
				for(int j=OT_Pos[rule[i]-'0'];j<OT_Pos[rule[i]-'0'+1];j++)
				{
                    if(BS==0)
					    arr_out[BS*51+j]=0;
                    else if(BS==1)
                        arr_out[BS*51+j]=2;
				}
			}
		}
		if(rule[i]>='a'&&rule[i]<='z')
		{
            if(BS==0)
			    arr_out[BS*51+OT_Pos[num-'0']+trans_rev_lookup[rule[i]]]=1-minusflag;
            else if(BS==1)
			    arr_out[BS*51+OT_Pos[num-'0']+trans_rev_lookup[rule[i]]]=1+minusflag;
		}
	}
    if(arr_out[0]==1)
    {
        cout<<"B0 is not allowed in this space."<<endl;
        exit(1);
        arr_out[0]=0;
    }
	return arr_out;
}

string rulestring_from_arr(RuleRep& rule)
{
	string out="";
	for(int i=0;i<9;i++)
	{
		int pos_flag=0;
		vector<char> tmp1,tmp2;
		for(int j=OT_Pos[i];j<OT_Pos[i+1];j++)
		{
			if(rule[j+51]==1)
			{
				tmp1.push_back(trans_name_lookup[j][1]);
			}
			else
			{
				tmp2.push_back(trans_name_lookup[j][1]);
			}
		}
		if(tmp1.size()==0) 
		{
			out+="";
		}
		else if (tmp1.size()==OT_Pos[i+1]-OT_Pos[i])
		{
			out+=(i+'0');
		}
		else if (tmp1.size()<=tmp2.size())
		{
			out+=(i+'0');
			sort(tmp1.begin(),tmp1.end());
			for(char c:tmp1) out+=c;
		}
		else
		{
			out+=(i+'0');
			out+="-";
			sort(tmp2.begin(),tmp2.end());
			for(char c:tmp2) out+=c;
		}
	}
	out+="/";
    for(int i=0;i<9;i++)
	{
		int pos_flag=0;
		vector<char> tmp1,tmp2;
		for(int j=OT_Pos[i];j<OT_Pos[i+1];j++)
		{
			if(rule[j]==1)
			{
				tmp1.push_back(trans_name_lookup[j][1]);
			}
			else
			{
				tmp2.push_back(trans_name_lookup[j][1]);
			}
		}
		if(tmp1.size()==0) 
		{
			out+="";
		}
		else if (tmp1.size()==OT_Pos[i+1]-OT_Pos[i])
		{
			out+=(i+'0');
		}
		else if (tmp1.size()<=tmp2.size())
		{
			out+=(i+'0');
			sort(tmp1.begin(),tmp1.end());
			for(char c:tmp1) out+=c;
		}
		else
		{
			out+=(i+'0');
			out+="-";
			sort(tmp2.begin(),tmp2.end());
			for(char c:tmp2) out+=c;
		}
	}
    out+="/";
    for(int i=TRANSCOUNT-1;i>=102;i--)
    {
        if(rule[i]) 
        {
            out+=to_string(i-98);
            return out;
        }
    }
    out+="3";
	return out;
}

unsigned long long int check_frontend(RulespaceRep& rulespace, CellArray& arr)
{
	bool b1e = !!(rulespace[2][0]==-1);
	bool b2a = !!(rulespace[6][0]==-1);
	bool b2c = !!(rulespace[3][0]==-1);
	bool b3i = !!(rulespace[13][0]==-1);

	int ind = ((int)b3i*8)+((int)b2c*4)+(int)b2a*2+(int)b1e;

	static int off[16] = {-1,1,1,-1,-1,2,2,-1,-1,1,0,-1,-1,2,0,-1};

	if(rulespace[1][0]==-1) return 15;
	if (!(b1e||b2a)) return 0;
	if (b1e&&b2a) return 15;
	int offmargin=off[ind];
	int onmargin=b1e?1:2;

	static array<int,3> defqueue = {-1,-1,LORGE};
	array<int,3> queue = defqueue;
	bool state=0;
	int nxt;

	int bx=0, by=0;

	unsigned long long int out=0;
	bool flg=false;
	for(int side=0;side<4;side++)
	{
		bx=(side&2)?RANGE:RANGE+arr.dims_x-1;
		by=(side&2)?RANGE+arr.dims_y-1:RANGE;
		for(int c=RANGE;c<((side&1)?arr.dims_x:arr.dims_y)+RANGE;c++)
		{
			nxt=1-(bool)(arr.cells[(side&1)?c:bx][(side&1)?by:c]-1);
			if(nxt!=state)
			{
				queue[0]=queue[1];
				queue[1]=queue[2];
				queue[2]=1;
				state=!state;
			}
			else
			{
				queue[2]++;
			}
			if (!state && queue[0]>=offmargin && (queue[1]==onmargin||(queue[1]>onmargin&&b2a&&b3i)) && queue[2]>=offmargin)
			{
				out|=(1<<side);
				flg=true;
				break;
			}
		}
		if (!flg)
		{
			if(state)
			{
				queue[0]=queue[1];
				queue[1]=queue[2];
				state=!state;
			}
			queue[2]=LORGE;
			if (!state && queue[0]>=offmargin && (queue[1]==onmargin||(queue[1]>onmargin&&b2a&&b3i)) && queue[2]>=offmargin)
			{
				out|=(1<<side);
			}
		}
		flg=false;
		queue=defqueue; 
		state=0;
	}
	return out;
}

bool frontend_allowed_for_stable(unsigned long long frontend)
{
	return (frontend==0||frontend==1||frontend==2||frontend==4||frontend==8);
}

RulespaceRep maxspace = rulespace_from_rules(vector<RuleRep>({parse_rule("//3"),parse_rule("012345678/12345678/25")}));