#include "arrayfuncs.h"
#include "parseinput.h"

//totalarray cell values are indexed by [starting pattern][gen].cells[x][y]
vector<vector<CellArray>> totalarray;
vector<CellArray> targets;
vector<CellArray> slidingmasks;
vector<CellArray> staticmasks;
vector<CellArray> slidingcomps;
vector<CellArray> staticcomps;
int pattcount;
int targetcount;
int maxdepth;

#ifndef LINKED
int comm=TRANSCOUNT;
#else
int comm=LINKED;
#endif

//Current rulespace we are working in
RulespaceRep rulespace;
//Boolean arrays of new transitions
TransSetArray toupdatetranstmp;
//Basically for lists of new transitions
vector<TransArray> newtransarrtmp;

print_mode cellprintmode;
ofstream outstream;

vector<bool> nulltarget;
vector<int> solvedgens;
vector<__int128> progressnumer;
vector<__int128> progressdenom;

long long unsigned int solutioncount;
long long unsigned int totalbranches;

stringstream tmpstring;

unordered_map<string,int> ssssstable;
vector<CellArray> ssssscanonizationarrays;

vector<vector<vector<vector<bool>>>> occupied;

string argvstr;

bool loading;

default_random_engine rng;

#ifdef CHECKFRONTEND
vector<unsigned long long int> frontend_bits;
#endif

//#define CUSTOMRESULT

inline bool custom_prune(CellArray& arr, int depth)
{
    //return ((depth<100||arr.pop<46)&&(depth<60||depth>=100||arr.pop<110-(depth-60)*1));
    return true;
}

#ifdef CUSTOMRESULT
inline bool custom_result(CellArray& arr, int depth)
{
    return depth>=10000||arr.dims_x>=100;
}
#endif

inline string result_string(int newdepth)
{
    stringstream s;
    RuleRep mr = minimum_rule(rulespace);
    for(int i=0;i<pattcount;i++)
    {
        if(i<targetcount)
        {
            s<<", "<<mr<<", , , "<<solvedgens[i]<<", ";
            // #if STATECOUNT==2||defined TWOSTATEOUTPUT
            // s<<two_state_grid_to_RLE(totalarray[i][0])<<", "<<two_state_grid_to_RLE(targets[i])<<endl;
            // #else
            s<<multi_state_grid_to_RLE(totalarray[i][0])<<", "<<multi_state_grid_to_RLE(targets[i])<<endl;
            // #endif
        }
        else
        {
            int mpop=(LORGE);
            int tmp=0;
            int n = solvedgens[i];
            int p=totalarray[i][n].period;
            int mpopgen=-1;
            #ifndef CUSTOMRESULT
            for(int j=n-p;j<n;j++)
            {
                tmp = totalarray[i][j].pop;
                if(tmp<mpop)
                {
                    mpop=tmp;
                    mpopgen=j;
                }
            }
            #else
            mpopgen=0;
            #endif
            CellArray* printedarrayp;
            if(OptionState::sssssmode)
            {
                int ax=totalarray[i][n].disp_x-totalarray[i][n-p].disp_x;
                int ay=totalarray[i][n].disp_y-totalarray[i][n-p].disp_y;
                s<<mpop<<", "<<mr<<", "<<canonized_speed(ax,ay,p)<<", ";
                canonize_array(totalarray[i][mpopgen],ssssscanonizationarrays[i],ax,ay);
                printedarrayp=&(ssssscanonizationarrays[i]);
            }
            else
            {
                s<<mpop<<", "<<mr<<", "<<\
                totalarray[i][n].disp_x-totalarray[i][n-p].disp_x<<", "<<\
                totalarray[i][n].disp_y-totalarray[i][n-p].disp_y<<", "<<p<<", ";
                printedarrayp=&(totalarray[i][mpopgen]);
            }
            CellArray& printedarray=*printedarrayp;

            #if STATECOUNT==2
            s<<two_state_grid_to_RLE(printedarray)<<endl;
            #elif defined TWOSTATEOUTPUT
            string w = two_state_grid_to_RLE(printedarray);
            if(w!="!")
                s<<w<<endl;
            else
                s<<multi_state_grid_to_RLE(printedarray)<<endl;
            #else
            s<<multi_state_grid_to_RLE(printedarray)<<endl;
            #endif
        }
    }
    if(pattcount>1) s<<endl;
    return s.str();
}

//Currentdepth+1
inline bool check_valid(int newdepth)
{
    //Smaller checks first, likelier to be hit earlier before the long computations of comparing arrays
    for(int patt=0;patt<pattcount;patt++)
    {
        if(solvedgens[patt]==-1)
        {
            CellArray& newarray = totalarray[patt][newdepth];
            if(newarray.pop==0)
            {
                #ifdef CUSTOMRESULT
                return 0;
                #endif
                if(nulltarget[patt])
                {
                    solvedgens[patt]=newdepth;
                    continue;
                }
                else
                {
                    return 0;
                }
            }
            if(newarray.dims_x>newarray.max_x||newarray.dims_y>newarray.max_y) return 0;
            #ifdef CHECKFRONTEND
            if(patt>=targetcount && !OptionState::noexp[patt])
            {
                unsigned long long int newfront=check_frontend(rulespace, newarray);
                if(!frontend_allowed_for_stable(newfront)||newfront!=frontend_bits[patt]) return 0;
            }
            #endif
            if(!OptionState::nosymm[patt] && !symmetry_check(newarray))
            {
                if(patt>=targetcount)
                {
                    return 0;
                }
                else
                {
                    newarray.symm=determine_symm(newarray);
                    if(!possible_symm_transition(newarray.symm,targets[patt].symm))
                    {
                        return 0;
                    }
                }
            }
            if(OptionState::slidingmask.size() && !check_mask(newarray, slidingmasks[patt], 0, 0)) return 0;
            if(OptionState::staticmask.size() && !check_mask(newarray, staticmasks[patt], newarray.disp_x-OptionState::staticmaskoff[patt].first, newarray.disp_y-OptionState::staticmaskoff[patt].second)) return 0;
            if(newarray.pop<OptionState::prune_minpop[patt]||newarray.pop>OptionState::prune_maxpop[patt])
            {
                return 0;
            }
            if(newarray.disp_x<OptionState::prune_mindispx[patt]||newarray.disp_x>OptionState::prune_maxdispx[patt])
            {
                return 0;
            }
            if(newarray.disp_y<OptionState::prune_mindispy[patt]||newarray.disp_y>OptionState::prune_maxdispy[patt])
            {
                return 0;
            }
            if(comm<OptionState::prune_mincomm[patt]||comm>OptionState::prune_maxcomm[patt])
            {
                return 0;
            }
            if(!custom_prune(newarray, newdepth))
            {
                return 0;
            }
            #ifdef CUSTOMRESULT
            if(custom_result(newarray, newdepth))
            {
                if(OptionState::enforce_move[patt] && newarray.disp_x==0 && newarray.disp_y==0)
                {
                    return 0;
                }
                if(OptionState::minperiod>newdepth)
                {
                    return 0;
                }
                if(newarray.disp_x<OptionState::filter_mindispx[patt]||newarray.disp_x>OptionState::filter_maxdispx[patt])
                {
                    return 0;
                }
                if(newarray.disp_y<OptionState::filter_mindispy[patt]||newarray.disp_y>OptionState::filter_maxdispy[patt])
                {
                    return 0;
                }
                if(newarray.pop<OptionState::filter_minpop[patt]||newarray.pop>OptionState::filter_maxpop[patt])
                {
                    return 0;
                }
                if(comm<OptionState::filter_mincomm[patt]||comm>OptionState::filter_maxcomm[patt])
                {
                    return 0;
                }
                solvedgens[patt]=newdepth;
                continue;
            }
            #endif
        }
    }

    for(int patt=0;patt<pattcount;patt++)
    {
        if(solvedgens[patt]==-1)
        {
            CellArray& newarray = totalarray[patt][newdepth];
            if(patt>=targetcount)
            {
                for(int gen=0;gen<newdepth;gen++)
                {
                    bool o=true;
                    if(gen==0 && (OptionState::slidingcomp.size()||OptionState::staticcomp.size()))
                    {
                        if(OptionState::staticcomp.size())
                        {
                            o*=compare_cell_arrays(newarray, totalarray[patt][0], staticcomps[patt], newarray.disp_x-OptionState::staticcompmaskoff[patt].first, newarray.disp_y-OptionState::staticcompmaskoff[patt].second);
                        }
                        if(OptionState::slidingcomp.size())
                        {
                            o*=compare_cell_arrays(newarray, totalarray[patt][0], slidingcomps[patt], 0, 0);
                        }
                        if(!o)
                        {
                            if(compare_cell_arrays(newarray, totalarray[patt][0])) return 0;
                        }
                    }
                    else
                    {
                        o=compare_cell_arrays(newarray, totalarray[patt][gen]);
                    }
                    if(o)
                    {   
                        #ifdef CUSTOMRESULT
                        return 0;
                        #endif             
                        if(gen!=0&&OptionState::allow_evolve[patt]==0)
                        {
                            return 0;
                        }
                        int adisp_x=newarray.disp_x-totalarray[patt][gen].disp_x;
                        int adisp_y=newarray.disp_y-totalarray[patt][gen].disp_y; 
                        if(OptionState::minperiod>newdepth-gen)
                        {
                            return 0;
                        }
                        if(OptionState::enforce_move[patt] && adisp_x==0 && adisp_y==0)
                        {
                            return 0;
                        }
                        if(adisp_x<OptionState::filter_mindispx[patt]||adisp_x>OptionState::filter_maxdispx[patt])
                        {
                            return 0;
                        }
                        if(adisp_y<OptionState::filter_mindispy[patt]||adisp_y>OptionState::filter_maxdispy[patt])
                        {
                            return 0;
                        }
                        if(comm<OptionState::filter_mincomm[patt]||comm>OptionState::filter_maxcomm[patt])
                        {
                            return 0;
                        }
                        int tmp;
                        int mpop=(LORGE);
                        for(int j=gen;j<newdepth;j++)
                        {
                            tmp = totalarray[patt][j].pop;
                            if(tmp<mpop)
                            {
                                mpop=tmp;
                            }
                        }
                        if(mpop<OptionState::filter_minpop[patt]||mpop>OptionState::filter_maxpop[patt])
                        {
                            return 0;
                        }
                        if(OptionState::sssssmode && newdepth-gen)
                        {
                            string c = canonized_speed(adisp_x,adisp_y,newdepth-gen);
                            if(ssssstable.count(c) && ssssstable[c]<=mpop)
                            {
                                return 0;
                            }
                            else
                            {
                                ssssstable[c]=mpop;
                            }
                        }
                        solvedgens[patt]=newdepth;
                        totalarray[patt][newdepth].period=newdepth-gen;
                    }
                }
            }
            else
            {
                bool o=true;
                if(OptionState::slidingcomp.size()||OptionState::staticcomp.size())
                {
                    if(OptionState::staticcomp.size())
                    {
                        o*=compare_cell_arrays(newarray, targets[patt], staticcomps[patt], newarray.disp_x-OptionState::staticcompmaskoff[patt].first, newarray.disp_y-OptionState::staticcompmaskoff[patt].second);
                    }
                    if(OptionState::slidingcomp.size())
                    {
                        o*=compare_cell_arrays(newarray, targets[patt], slidingcomps[patt], 0, 0);
                    }
                }
                else
                {
                    o=compare_cell_arrays(newarray, targets[patt]);
                }
                if(o)
                {
                    if(OptionState::enforce_move[patt] && newarray.disp_x==0 && newarray.disp_y==0)
                    {
                        return 0;
                    }
                    if(OptionState::minperiod>newdepth)
                    {
                        return 0;
                    }
                    if(newarray.disp_x<OptionState::filter_mindispx[patt]||newarray.disp_x>OptionState::filter_maxdispx[patt])
                    {
                        return 0;
                    }
                    if(newarray.disp_y<OptionState::filter_mindispy[patt]||newarray.disp_y>OptionState::filter_maxdispy[patt])
                    {
                        return 0;
                    }
                    if(newarray.pop<OptionState::filter_minpop[patt]||newarray.pop>OptionState::filter_maxpop[patt])
                    {
                        return 0;
                    }
                    if(comm<OptionState::filter_mincomm[patt]||comm>OptionState::filter_maxcomm[patt])
                    {
                        return 0;
                    }
                    #ifndef CUSTOMRESULT
                    solvedgens[patt]=newdepth;
                    #endif
                }
                for(int gen=0;gen<newdepth;gen++)
                {
                    if(solvedgens[patt]!=newdepth&&compare_cell_arrays(newarray, totalarray[patt][gen]))
                    {
                        return 0;
                    }
                }
            }
        }
    }

    for(int patt=0;patt<pattcount;patt++)
    {
        if(solvedgens[patt]==-1)
        {
            return 1;
        }
    }
    solutioncount++;
    tmpstring<<result_string(newdepth);
    return 0;
}

void print_progress(int currentdepth)
{
    cout<<"Depth: "<<currentdepth;
    cout<<", Progress: ";
    int onecount=0;
    for (int i=0;i<currentdepth+1;i++)
    {
        if(progressdenom[i]==1) onecount++;
        else onecount=0;
        if(onecount&&progressdenom[i+1]==1) continue;
        else 
        {
            cout<<"["<<progressnumer[i]<<"/"<<progressdenom[i]<<"]"<<(onecount>1?"*"+to_string(onecount):"");
            onecount=0;
        }
        if(i!=currentdepth) cout<<",";
    }
    if(onecount) cout<<"[0/1]"<<(onecount>1?"*"+to_string(onecount):"");
    cout<<endl;
    //cout<<totalarray[0][currentdepth+1]<<endl;
    cout<<"Solutions: "<<solutioncount<<endl;
    cout<<"Branches: "<<totalbranches<<endl<<endl;
}

void select_branch_evolve(int currentdepth, __int128 index, TransArray& newtrans)
{
    //For each new transition, set the rulespace's value there according to the partitioning of the index
    //It's set to the negative of the new state. This is to avoid having yet another array of stuff, but it looks kind of ridiculous
    //I'll probably hate this later.
    #if STATECOUNT==2
	for (int j=0;j<newtrans[TRANSCOUNT];j++)
	{
		rulespace[newtrans[j]][0] = -rulespace[newtrans[j]][(index%2)+1];
        #ifdef LINKED
        queue<pair<int,int>>& l = linked(newtrans[j],-rulespace[newtrans[j]][0]);
        pair<int,int> p;
        while(!l.empty())
        {
            p=l.front();
            l.pop();
            rulespace[p.first][0] = -p.second;
        }
        #endif
		index >>= 1;
	}
	#else
	int tmp=0;
	for (int j=0;j<newtrans[TRANSCOUNT];j++)
	{
        int tmp = rulespace[newtrans[j]].size()-1;
		rulespace[newtrans[j]][0] = -rulespace[newtrans[j]][(index%tmp)+1];
        #ifdef LINKED
        queue<pair<int,int>>& l = linked(newtrans[j],-rulespace[newtrans[j]][0]);
        pair<int,int> p;
        while(!(l.empty()))
        {
            p=l.front();
            l.pop();
            rulespace[p.first][0] = -p.second;
        }
        #endif
		index/=tmp;
	}
	#endif

    for(int i=0;i<pattcount;i++)
    {
        if(solvedgens[i]==-1)
        {
            evolve(rulespace, totalarray[i][currentdepth], totalarray[i][currentdepth+1]);
        }
    }
}

void branch(int currentdepth)
{
    while(currentdepth>=0)
    {
        outside:;
        //Clear these
        toupdatetranstmp.fill(0);
        newtransarrtmp[currentdepth].fill(0);

        __int128 newbranchcount=0;
        int j=0;

        //Generate trans cell arrays and the set of new transitions to branch from
        for(int i=0;i<pattcount;i++)
        {
            if(solvedgens[i]==-1)
            {
                if(currentdepth==OptionState::gendepths[i])
                {
                    currentdepth--;
                    goto inside;
                }
                totalarray[i][currentdepth].generate_trans_array(rulespace,toupdatetranstmp);
            }
        }
        for(int i=0;i<TRANSCOUNT;i++)
        {
            if(toupdatetranstmp[i])
            {
                newtransarrtmp[currentdepth][j++]=i;
            }
        }
        if(OptionState::randseed!=LORGE) shuffle(newtransarrtmp[currentdepth].begin(), newtransarrtmp[currentdepth].begin()+j,rng);
        newtransarrtmp[currentdepth][TRANSCOUNT]=j;
        comm-=j;

        //Calculate number of branches
        #if STATECOUNT==2
        newbranchcount = (__int128)1<<newtransarrtmp[currentdepth][TRANSCOUNT];
        #else
        newbranchcount=1;
        for(int i=0;i<newtransarrtmp[currentdepth][TRANSCOUNT];i++)
        {
            newbranchcount*=rulespace[newtransarrtmp[currentdepth][i]][0];
            //Cap this so we don't get weird overflows or stupid stuff
            if(newbranchcount>((__int128)1<<120))
            {
                cout<<"Maximum branch count exceeded."<<endl;
                //Probably should exit as it's not like anyone's going to be fully running 10^36 branches anyway
                exit(1);
                break;
            }
        }
        #endif

        progressdenom[currentdepth]=newbranchcount;
        //Loop over all branches
        progressnumer[currentdepth]=0;
        if(loading) progressnumer[currentdepth]=(__int128)OptionState::startingbranchoffset[currentdepth];
        if(OptionState::startingbranchoffset.size()<=currentdepth+1) loading=false;
        for(;progressnumer[currentdepth]<progressdenom[currentdepth];progressnumer[currentdepth]++)
        {
            totalbranches++;
            for(auto& i:solvedgens)
            {
                if(i>currentdepth) i=-1;
            }
            select_branch_evolve(currentdepth, progressnumer[currentdepth], newtransarrtmp[currentdepth]);
            #ifdef CHECKFRONTEND
            if(currentdepth==0)
            {
                for(int i=targetcount;i<pattcount;i++)
                {
                    frontend_bits[i]=check_frontend(rulespace, totalarray[i][0]);
                }
            }
            #endif
            //Print tree progress bracket thing and output to file
            if(totalbranches&&totalbranches%OptionState::reporting_interval==0)
            {
                outstream<<tmpstring.str();
                outstream<<"#"<<totalbranches<<" branches"<<endl;
                tmpstring.str("");
                if(OptionState::savefile.size())
                {
                    ofstream savestream(OptionState::savefile);
                    if(!!(savestream))
                    {
                        savestream<<argvstr<<endl;
                        for(int q=0;q<currentdepth+1;q++)
                        {
                            savestream<<progressnumer[q]<<" ";
                        }
                    }
                    savestream.close();
                }
                print_progress(currentdepth);
            }

            //If all checks pass, recurse
            if(check_valid(currentdepth+1))
            {
                currentdepth++;
                goto outside;
            }
            inside:;
        }
        //Reset rulespace modifications
        for(int i=0;i<newtransarrtmp[currentdepth][TRANSCOUNT];i++)
        {
            rulespace[newtransarrtmp[currentdepth][i]][0]=rulespace[newtransarrtmp[currentdepth][i]].size()-1;
            #ifdef LINKED 
            queue<pair<int,int>>& l = linked(newtransarrtmp[currentdepth][i],0);
            pair<int,int> p;
            while(!l.empty())
            {
                p=l.front();
                l.pop();
                rulespace[p.first][0] = rulespace[p.first].size()-1;
            }
            #endif
        }
        comm+=newtransarrtmp[currentdepth][TRANSCOUNT];
        currentdepth--;
        if(currentdepth<0) break;
        goto inside;
    }
}

int main(int argc, char** argv)
{
    loading=true;
    chrono::steady_clock::time_point t1=chrono::steady_clock::now();

    //Set up print mode and parse inputs
    cellprintmode = print_mode::mode_cell;
    parse_input(argc,argv);
    check_parsed();

    tmpstring=stringstream();
    solutioncount=0;
    totalbranches=0;
    maxdepth = *max_element(OptionState::gendepths.begin(), OptionState::gendepths.end());
    pattcount = OptionState::startingpatterns.size();
    targetcount = OptionState::targetpatterns.size();

    //Main arrays
    totalarray.resize(pattcount);
    newtransarrtmp.resize(maxdepth+1,{0});
    nulltarget.resize(pattcount,0);
    solvedgens.resize(pattcount,-1);
    progressnumer.resize(maxdepth+1,0);
    progressdenom.resize(maxdepth+1,0);

    #ifdef CHECKFRONTEND
    frontend_bits.resize(pattcount);
    #endif

    vector<RuleRep> rulespaceparts;
    RulespaceRep initialrulespace;

    #ifdef CUSTOMRESULT
    cout<<"Warning: Using custom function for result reporting."<<endl;
    #endif

    rng = default_random_engine {(unsigned int) OptionState::randseed};

    //Initialize all starting patterns
    for(int i=0;i<pattcount;i++)
    {
        CellArray c = CellArray(OptionState::xdims[i],OptionState::ydims[i],i);
        totalarray[i].resize(OptionState::gendepths[i]+1, c);
        set_grid(totalarray[i][0], OptionState::startingpatterns[i]);
        if(OptionState::sssssmode)
        {
            int maxdim=max(OptionState::xdims[i],OptionState::ydims[i]);
            ssssscanonizationarrays.push_back(CellArray(maxdim,maxdim,-1));
        }
    }

    //Create all target patterns
    for(int i=0;i<targetcount;i++)
    {
        targets.push_back(CellArray(OptionState::xdims[i],OptionState::ydims[i],-1));
        set_grid(targets[i], OptionState::targetpatterns[i]);
        //If target is empty
        if(targets[i].pop==0)
        {
            nulltarget[i]=1;
        }
    }

    if(OptionState::slidingmask.size())
    {
        for(int i=0;i<pattcount;i++)
        {
            slidingmasks.push_back(CellArray(OptionState::xdims[i],OptionState::ydims[i],-1));
            set_grid(slidingmasks[i], OptionState::slidingmask[i],false);
        }
    }

    if(OptionState::staticmask.size())
    {
        for(int i=0;i<pattcount;i++)
        {
            CellArray c = CellArray(0,0,SMOLL);
            pair<int,int> pr = set_grid(c,OptionState::staticmask[i]);
            staticmasks.push_back(CellArray(pr.first,pr.second,-1));
            set_grid(staticmasks[i], OptionState::staticmask[i]);
        }
    }

    if(OptionState::slidingcomp.size())
    {
        for(int i=0;i<pattcount;i++)
        {
            slidingcomps.push_back(CellArray(OptionState::xdims[i],OptionState::ydims[i],-1));
            set_grid(slidingcomps[i], OptionState::slidingcomp[i],false);
        }
    }

    if(OptionState::staticcomp.size())
    {
        for(int i=0;i<pattcount;i++)
        {
            CellArray c = CellArray(0,0,SMOLL);
            pair<int,int> pr = set_grid(c,OptionState::staticcomp[i]);
            staticcomps.push_back(CellArray(pr.first,pr.second,-1));
            set_grid(staticcomps[i], OptionState::staticcomp[i]);
        }
    }

    for(auto rule:OptionState::rulespace)
    {
        rulespaceparts.push_back(parse_rule(rule));
    }

    //Set initial rulespace
    initialrulespace = rulespaceparts.size()>0?rulespace_from_rules(rulespaceparts):maxspace;

    for(int i=0;i<TRANSCOUNT;i++)
    {
        if(initialrulespace[i][0]<1&&OptionState::incomingrulespace[i][0]<1)
        {
            if(initialrulespace[i][0]!=OptionState::incomingrulespace[i][0])
            {
                cout<<"Rulespaces are conflicting."<<endl;
                exit(1);
            }
        }
        if(initialrulespace[i][0]>=1&&OptionState::incomingrulespace[i][0]<1)
        {
            initialrulespace[i][0]=OptionState::incomingrulespace[i][0];
        }
    }
    rulespace=initialrulespace;

    for(int i=0;i<pattcount;i++)
    {
        cout<<"Bounding box: " << totalarray[i][0].dims_x<<"x"<< totalarray[i][0].dims_y<<endl;
        cout<<totalarray[i][0]<<endl;
    }

    if(OptionState::append)
    {
        outstream.open(OptionState::filename,std::ios_base::app);
    }
    else
    {
        outstream.open(OptionState::filename);
    }

    stringstream tmps;

    tmps<<"./EPE ";
    for(int i=1;i<argc;i++)
    {
        tmps<<"'"<<argv[i]<<"' ";
    }
    
    argvstr = OptionState::loadedargv.size()?OptionState::loadedargv:tmps.str();

    outstream<<"#"<<argvstr<<endl;

    if(OptionState::sssssfolder!="")
    {
        load_5s(ssssstable, OptionState::sssssfolder);
        #ifndef INT_H
        cout<<"You may be loading INT speeds in a non-INT rulespace, beware!"<<endl;
        #endif
    }

    branch(0);
    outstream<<tmpstring.str();
    cout << "time: " << chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - t1).count() << " seconds"<<endl;
    cout<<"Solutions found: "<<solutioncount<<endl;
    cout<<"Branches traversed: "<<totalbranches<<endl;
}