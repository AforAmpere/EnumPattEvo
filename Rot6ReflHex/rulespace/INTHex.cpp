#include "src/arrayfuncs.h"

const int trans_lookup[128] = {
0,1,1,2,1,2,3,5,13,14,14,15,14,15,16,18,1,3,2,5,4,6,6,8,
14,16,15,18,17,19,19,21,1,3,4,6,2,5,6,8,14,16,17,19,15,
18,19,21,3,7,6,9,6,9,10,11,16,20,19,22,19,22,23,24,1,4,
3,6,3,6,7,9,14,17,16,19,16,19,20,22,2,6,5,8,6,10,9,11,
15,19,18,21,19,23,22,24,2,6,6,10,5,8,9,11,15,19,19,23,
18,21,22,24,5,9,8,11,8,11,11,12,18,22,21,24,21,24,24,25
};

const char *trans_name_lookup[13]={
"0o",
"1o",
"2o","2m","2p",
"3o","3m","3p",
"4o","4m","4p",
"5o",
"6o"
};

unordered_map<char, int> trans_rev_lookup=
{
	{'o',0},
	{'m',1},
	{'p',2},
};
const int OT_Pos[8]={0,1,2,5,8,11,12,13};

int CellArray::get_cell_trans(int x, int y)
{
    return 
    trans_lookup[((x==0 || y==0) ? 0:cells[x-1][y-1])+\
    (y==0 ? 0:(cells[x][y-1])<<1)+\
    (x==0 ? 0:(cells[x-1][y])<<2)+\
    ((cells[x][y])<<3)+\
    ((cells[x+1][y])<<4)+\
    ((cells[x][y+1])<<5)+\
    ((cells[x+1][y+1])<<6)];
}

//string reg = "[BbSs]((0|1-[ce]+|1[ce]*|2-[aceikn]+|2[aceikn]*|3-[aqrceiyjkn]+|3[aqrceiyjkn]*|4-[aceijknqrtwyz]+|4[aceijknqrtwyz]*|5-[aqrceiyjkn]+|5[aqrceiyjkn]*|6-[aceikn]+|6[aceikn]*|7-[ce]+|7[ce]*|8))*[_/]?[BbSs]((0|1-[ce]+|1[ce]*|2-[aceikn]+|2[aceikn]*|3-[aqrceiyjkn]+|3[aqrceiyjkn]*|4-[aceijknqrtwyz]+|4[aceijknqrtwyz]*|5-[aqrceiyjkn]+|5[aqrceiyjkn]*|6-[aceikn]+|6[aceikn]*|7-[ce]+|7[ce]*|8))*";

RuleRep parse_rule(string rule)
{
	if (rule.size()==0)
	{
		cout<<"Rule: "<<rule<<" is invalid."<<endl;
		exit(1);
	}
    RuleRep arr_out={0};
	int BS=-1;
	int minusflag=0;
	char num = '#';
	if(rule[0]=='B')
	{
		BS=0;
	}
	else
	{
		BS=1;
	}
	for(int i=0;i<rule.size()-1;i++)
	{
		if(rule[i]=='/')
		{
			BS=1-BS;
		}
		if(rule[i]=='S'||rule[i]=='s')
		{
			BS=1;
		}
		if(rule[i]>='0'&&rule[i]<='9')
		{
			num=rule[i];
			minusflag=0;
			if(i==rule.size()-2||(rule[i+1]>='0'&&rule[i+1]<='9')||rule[i+1]=='-'||rule[i+1]=='/'||rule[i+1]=='s'||rule[i+1]=='S')
			{
				if(rule[i+1]=='-') minusflag=1;
				for(int j=OT_Pos[rule[i]-'0'];j<OT_Pos[rule[i]-'0'+1];j++)
				{
					arr_out[BS*13+j]=1;
				}
			}
			else
			{
				for(int j=OT_Pos[rule[i]-'0'];j<OT_Pos[rule[i]-'0'+1];j++)
				{
					arr_out[BS*13+j]=0;
				}
			}
		}
		if(rule[i]>='a'&&rule[i]<='z'&&rule[i]!='s'&&rule[i]!='b')
		{
			arr_out[BS*13+OT_Pos[num-'0']+trans_rev_lookup[rule[i]]]=1-minusflag;
		}
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
	string out="B";
	for(int i=0;i<7;i++)
	{
		//int pos_flag=0;
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
	out+="/S";
	for(int i=0;i<7;i++)
	{
		//int pos_flag=0;
		vector<char> tmp1,tmp2;
		for(int j=OT_Pos[i];j<OT_Pos[i+1];j++)
		{
			if(rule[j+13]==1)
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
    out+="H";
	return out;
}

RulespaceRep maxspace = rulespace_from_rules(vector<RuleRep>({parse_rule("B/S"),parse_rule("B123456/S0123456H")}));