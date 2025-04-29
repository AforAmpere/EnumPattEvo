#include "src/arrayfuncs.h"

int CellArray::get_cell_trans(int x, int y)
{
    int p=0;
    for(int i=std::max(0,x-RANGE);i<=x+RANGE;i++)
    {
        for(int j=std::max(0,y-RANGE);j<=y+RANGE;j++)
        {
            p+=cells[i][j];
        }
    }
    p+=(TRANSCOUNT/2-1)*cells[x][y];
    return p;
}

string reg = "R[0-9]+,C0,S[0-9\\-,]*,B[0-9\\-,]*";

RuleRep parse_rule(string rule)
{
	if (rule.size()==0 || !regex_match(rule, regex(reg)))
	{
		cout<<"Rule: "<<rule<<" invalid."<<endl;
		exit(1);
	}
    RuleRep arr_out={0};
	int BS=-1;
	int minusflag=0;
	int minr=-1;
	int maxr=-1;
	for(int i=0;i<rule.size();i++)
	{
        if(rule[i]=='S')
        {
            BS=1;
			minr=-1;
        }
        else if(rule[i]=='B')
        {
            BS=0;
			minr=-1;
        }
		else if(rule[i]>='0'&&rule[i]<='9')
		{
			if(minr==-1) minr=0;
			minr=minr*10+rule[i]-'0';
			maxr=minr;
		}
		else if(rule[i]==',')
		{
			if(BS==-1)
				continue;
			if(minr!=-1)
			{
				for(int j=minr;j<=maxr;j++)
				{
					arr_out[j+BS*(TRANSCOUNT/2)]=1;
				}
			}
			minr=-1;
			maxr=-1;
		}
		else if(rule[i]=='-')
		{
			maxr=0;
			for(int j=i+1;j<rule.size();j++)
			{
				if(rule[j]>='0'&&rule[j]<='9')
				{	
					maxr=maxr*10+rule[j]-'0';
				}
				else
				{
					i=j-1;
					break;
				}
				if(j==rule.size()-1)
				{
					i=rule.size();
					break;
				}
			}
		}
	}
	if(minr!=-1)
	{
		for(int j=minr;j<=maxr;j++)
		{
			arr_out[j+BS*(TRANSCOUNT/2)]=1;
		}
	}
	minr=-1;
	maxr=-1;
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
	stringstream out;
	out<<"R"<<RANGE<<",C0,S";
	int run=0;
	bool enc=false;
	for(int i=(TRANSCOUNT/2);i<TRANSCOUNT;i++)
	{
		if(rule[i]==1)
		{
			if(enc)
				out<<",";
			enc=true;
			int q=i;
			out<<i-(TRANSCOUNT/2);
			while(rule[i++]==1&&i<=TRANSCOUNT){}
			i-=1;
			if(q!=i-1)
			{
				out<<"-"<<i-(TRANSCOUNT/2)-1;
			}
		}
	}
	enc=false;
	out<<",B";
	
	for(int i=0;i<(TRANSCOUNT/2);i++)
	{
		if(rule[i]==1)
		{
			if(enc)
				out<<",";
			enc=true;
			int q=i;
			out<<i;
			while(rule[i++]==1&&i<=TRANSCOUNT/2){}
			i-=1;
			if(q!=i-1)
			{
				out<<"-"<<i-1;
			}
		}
	}
	return out.str();
}

RulespaceRep maxspace = rulespace_from_rules(vector<RuleRep>({parse_rule("R"+to_string(RANGE)+",C0,S,B"),parse_rule("R"+to_string(RANGE)+",C0,S0-"+to_string(TRANSCOUNT/2-1)+",B1-"+to_string(TRANSCOUNT/2-1))}));