#include "arrayfuncs.h"

CellArray::CellArray(int x, int y, int inpatt)
{
    cells.resize(x+3*RANGE, vector<int>(y+3*RANGE,0));
    trans.resize(x+3*RANGE, vector<int>(y+3*RANGE,0));
    max_x=x;
    max_y=y;
    dims_x=0;
    dims_y=0;
    disp_x=0;
    disp_y=0;
    pop=0;
    symm=0;
    patt=inpatt;
    period=0;
}

pair<int,int> two_state_RLE_to_grid(CellArray& array, string RLE, bool shift)
{
    string unrolled="";
    int num=-1;
    for(int i=0;i<RLE.length();i++)
    {
        if(RLE[i]>='0'&&RLE[i]<='9')
        {
            if(num!=-1) num=num*10;
            else num=0;
            num+=RLE[i]-'0';
        }
        else
        {
            for(int j=0;j<(num!=-1?num:1);j++)
            {
                unrolled+=RLE[i];
            }
            num=-1;
        }
    }

    int xp=0;
    int yp=0;
    int minx=-1;
    int miny=-1;
    for(char c:unrolled)
    {
        if(c=='o')
        {
            minx=(xp<minx||minx==-1)?xp:minx;
            miny=(yp<miny||miny==-1)?yp:miny;
            xp++;
            array.pop+=1;
        }
        else if(c=='b') 
        {
            xp++;
        }
        else if(c=='$') 
        {
            xp=0;
            yp++;
        }
    }
    if(minx==-1) return pair{0,0};
    if(!shift) {minx=0; miny=0;}
    xp=RANGE; yp=RANGE;
    int dims_x=0;
    int dims_y=0;
    for(char c:unrolled)
    {
        if(c=='o') 
        {
            if(dims_x<xp-minx+1-RANGE) dims_x=xp-minx+1-RANGE;
            if(dims_y<yp-miny+1-RANGE) dims_y=yp-miny+1-RANGE;
            if(array.patt!=SMOLL)
            {
                if(xp-minx<array.max_x+RANGE&&yp-miny<array.max_y+RANGE) array.cells[xp++-minx][yp-miny]=1;
                else
                {
                    cout<<"RLE too big for bounding box."<<endl;
                    exit(1);
                }
            }
            else
            {
                xp++;
            }
        }
        else if(c=='b') 
        {
            xp++;
        }
        else if(c=='$') 
        {
            yp++;
            xp=RANGE;
        }
        else if(c=='!') 
        {
            if(array.patt!=SMOLL)
            {
                array.dims_x=dims_x;
                array.dims_y=dims_y;
            }
            return pair{dims_x,dims_y};
        }
    }
    if(array.patt!=SMOLL)
    {
        array.dims_x=dims_x;
        array.dims_y=dims_y;
    }
    return pair{dims_x,dims_y};
}

//Supports state counts to 25
pair<int,int> multi_state_RLE_to_grid(CellArray& array, string RLE, bool shift)
{
    string unrolled="";
    int num=-1;
    for(int i=0;i<RLE.length();i++)
    {
        if(RLE[i]>='0'&&RLE[i]<='9')
        {
            if(num!=-1) num=num*10;
            else num=0;
            num+=RLE[i]-'0';
        }
        else
        {
            for(int j=0;j<(num!=-1?num:1);j++)
            {
                unrolled+=RLE[i];
            }
            num=-1;
        }
    }

    int xp=0;
    int yp=0;
    int minx=-1;
    int miny=-1;
    for(char c:unrolled)
    {
        if(c>='A'&&c<='X')
        {
            minx=(xp<minx||minx==-1)?xp:minx;
            miny=(yp<miny||miny==-1)?yp:miny;
            xp++;
            array.pop+=1;
        }
        else if(c=='.') 
        {
            xp++;
        }
        else if(c=='$') 
        {
            xp=0;
            yp++;
        }
    }
    if(minx==-1) return pair{0,0};
    if(!shift) {minx=0; miny=0;}
    xp=RANGE; yp=RANGE;
    int dims_x=0;
    int dims_y=0;
    for(char c:unrolled)
    {
        if(c>='A'&&c<='X') 
        {
            if(dims_x<xp-minx+1-RANGE) dims_x=xp-minx+1-RANGE;
            if(dims_y<yp-miny+1-RANGE) dims_y=yp-miny+1-RANGE;
            if(array.patt!=SMOLL)
            {
                if(xp-minx<array.max_x+RANGE&&yp-miny<array.max_y+RANGE) array.cells[xp++-minx][yp-miny]=c-'A'+1;
                else
                {
                    cout<<"RLE too big for bounding box."<<endl;
                    exit(1);
                }
            }
            else
            {
                xp++;
            }
        }
        else if(c=='.') 
        {
            xp++;
        }
        else if(c=='$') 
        {
            yp++;
            xp=RANGE;
        }
        else if(c=='!') 
        {
            if(array.patt!=SMOLL)
            {
                array.dims_x=dims_x;
                array.dims_y=dims_y;
            }
            return pair{dims_x,dims_y};
        }
    }
    if(array.patt!=SMOLL)
    {
        array.dims_x=dims_x;
        array.dims_y=dims_y;
    }
    return pair{dims_x,dims_y};
}

pair<int,int> set_grid(CellArray& refgrid, string RLE, bool shift)
{
    clear_grid(refgrid.cells, refgrid.dims_x, refgrid.dims_y);
    pair<int,int> p;
    #if STATECOUNT==2 || defined TWOSTATEOUTPUT
    p=two_state_RLE_to_grid(refgrid,RLE,shift);
    #else
    p=multi_state_RLE_to_grid(refgrid,RLE,shift);
    #endif

    refgrid.symm=determine_symm(refgrid);
    return p;
}

ostream& operator<< (ostream& os, __int128 lrgint)
{
    if(lrgint==0)
	{
		os<<0;
		return os;
	}
    string out_str,astr,f;
    while (lrgint != 0)
	{
        long long a = lrgint % (long long)pow(10,18);
        astr = to_string(a);
        f = (a != lrgint) ? string(18 - astr.length(), '0') : "";
        out_str = f + astr + out_str;
        lrgint = (lrgint - a) / (long long)pow(10,18);
    }
    os<<out_str;
	return os;
}

ostream& operator<< (ostream& os, CellArray& arr)
{
    int ly = RANGE;
    int lx = RANGE;
    int hy = arr.dims_y+RANGE;
    int hx = arr.dims_x+RANGE;
    if(cellprintmode==print_mode::mode_trans)
    {
        ly-=RANGE;
        lx-=RANGE;
        hy+=RANGE;
        hx+=RANGE;
    }
	for(int y=ly;y<hy;y++)
	{
		for(int x=lx;x<hx;x++)
		{
            if(cellprintmode==print_mode::mode_cell)
            {
			    os<<arr.cells[x][y]<<" ";
            }
            else if(cellprintmode==print_mode::mode_trans)
            {
                #ifdef INT_H
                os<<(arr.trans[x][y]/51?'S':'B')<<trans_name_lookup[arr.trans[x][y]%51]<<" ";
                #else
                os<<arr.trans[x][y]<<" ";
                #endif
            }
		}
		os<<endl;
	}
	return os;
}

ostream& operator<< (ostream& os, RuleRep& arr)
{
    os<<rulestring_from_arr(arr);
	return os;
}

ostream& operator<< (ostream& os, RuleRep&& arr)
{
    os<<rulestring_from_arr(arr);
	return os;
}

ostream& operator<< (ostream& os, TransArray& arr)
{
    for(int i=0;i<arr[TRANSCOUNT];i++)
    {
        #ifdef INT_H
        os<<(arr[i]/51?'S':'B')<<trans_name_lookup[arr[i]%51]<<",";
        #else
        os<<arr[i]<<",";
        #endif
    }
    os<<endl;
	return os;
}

ostream& operator<< (ostream& os, RulespaceRep& arr)
{
    for(int i=0;i<TRANSCOUNT;i++)
    {
        os<<"{";
        if(arr[i][0]>1)
        {
            for(int j=1;j<arr[i][0]+1;j++)
            {
                os<<arr[i][j];
                if(j!=arr[i][0])
                    os<<",";
            }
        }
        else
        {
            os<<-arr[i][0];
        }
        os<<"},";
    }
    os<<endl;
	return os;
}

RulespaceRep rulespace_from_rules(vector<RuleRep>& rules)
{
    RulespaceRep out;
    for(int i=0;i<TRANSCOUNT;i++)
    {
        long long unsigned int tmp=0;
        for(auto j:rules)
        {
            tmp|=(1<<j[i]);
        }
        for(int j=0;j<STATECOUNT;j++)
        {
            if(tmp&(1<<j))
            {
                out[i].push_back(j);
            }
        }
        #ifdef CUSTOMORDER
        sort(out[i].begin(),out[i].end(),[](int a, int b){array<int,1000> ordering =CUSTOMORDER; return ordering[a]<ordering[b];});
        #endif

        out[i].insert(out[i].begin(),out[i].size());
        if(out[i][0]==1) out[i][0]=-out[i][1];
    }
    return out;
}

RulespaceRep rulespace_from_rules(vector<RuleRep>&& rules)
{
    return rulespace_from_rules(rules);
}

RuleRep minimum_rule(RulespaceRep& rulespace)
{
    RuleRep out;
    int tmp;
    for(int i=0;i<TRANSCOUNT;i++)
    {
        tmp=rulespace[i][0];
        if(tmp<=0) out[i]=-tmp;
        else
        {
            #ifdef LINKED
            queue<pair<int,int>>& l = linked(i,rulespace[i][1]);
            pair<int,int> p;
            bool anybelow=false;
            while(!l.empty())
            {
                p=l.front();
                l.pop();
                if(p.first<i)
                {
                    anybelow=true;
                }
            }
            if(!anybelow)
            {
                out[i]=rulespace[i][1];
                l= linked(i,rulespace[i][1]);
                while(!l.empty())
                {
                    p=l.front();
                    l.pop();
                    out[p.first] = p.second;
                }
            }
            #else
            out[i]=rulespace[i][1];
            #endif
        }
    }
    return out;
}

string two_state_grid_to_RLE(CellArray& in_arr)
{
	if(in_arr.pop==0) return "b!";
	string RLE;

	int runningo=0,runningb=0,runningd=0;
	for(int j=RANGE;j<in_arr.dims_y+RANGE;j++)
	{
		for(int i=RANGE;i<in_arr.dims_x+RANGE;i++)
		{
			if(in_arr.cells[i][j]>0)
			{
				runningo++;
				if(runningd)
				{
					if(runningd>1)
						RLE+=to_string(runningd);
					RLE+="$";
					runningd=0;
				}
				if(runningb)
				{
					if(runningb>1)
						RLE+=to_string(runningb);
					RLE+="b";
					runningb=0;
				}
			}
			if(in_arr.cells[i][j]==0)
			{
				runningb++;
				if(runningo)
				{
					if(runningo>1)
						RLE+=to_string(runningo);
					RLE+="o";
					runningo=0;
				}
			}
		}
		if(runningo)
		{
			if(runningo>1)
				RLE+=to_string(runningo);
			RLE+="o";
			runningo=0;
		}
		runningb=0;
		runningd++;
		
	}
	if(runningo)
	{
		if(runningo>1)
			RLE+=to_string(runningo);
		RLE+="o";
		runningo=0;
	}
	if(runningb)
	{
		if(runningb<in_arr.dims_x+RANGE)
		{
			if(runningb>1)
				RLE+=to_string(runningb);
			RLE+="b";
		}
		runningb=0;
	}
	RLE+="!";
	return RLE;
}

//Buggy
string multi_state_grid_to_RLE(CellArray& in_arr)
{
	if(in_arr.pop==0) return ".!";
	string RLE;
	int runningo=0,runningb=0,runningd=0;
    int currst=0;
	for(int j=RANGE;j<in_arr.dims_y+RANGE;j++)
	{
		for(int i=RANGE;i<in_arr.dims_x+RANGE;i++)
		{
			if(in_arr.cells[i][j])
			{
                if(in_arr.cells[i][j]==currst)
                {
                    runningo++;
                }
                else
                {
                    if(runningo)
                    {
                        if(runningo>1)
                            RLE+=to_string(runningo);
                        RLE+=('A'+currst-1);
                    }
                    runningo=1;
                    currst=in_arr.cells[i][j];
                }
                if(runningd)
                {
                    if(runningd>1)
                        RLE+=to_string(runningd);
                    RLE+="$";
                    runningd=0;
                }
                if(runningb)
                {
                    if(runningb>1)
                        RLE+=to_string(runningb);
                    RLE+=".";
                    runningb=0;
                }
			}
			if(in_arr.cells[i][j]==0)
			{
				runningb++;
				if(runningo)
				{
					if(runningo>1)
						RLE+=to_string(runningo);
					RLE+=('A'+currst-1);
					runningo=0;
				}
			}
		}
		if(runningo)
		{
			if(runningo>1)
				RLE+=to_string(runningo);
			RLE+=('A'+currst-1);
			runningo=0;
		}
		runningb=0;
		runningd++;
		
	}
	if(runningo)
	{
		if(runningo>1)
			RLE+=to_string(runningo);
		RLE+=('A'+currst-1);
		runningo=0;
	}
	if(runningb)
	{
		if(runningb<in_arr.dims_x+RANGE)
		{
			if(runningb>1)
				RLE+=to_string(runningb);
			RLE+=".";
		}
		runningb=0;
	}
	RLE+="!";
	return RLE;
}