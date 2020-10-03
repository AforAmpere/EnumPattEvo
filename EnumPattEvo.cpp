#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>

#define MAXPOP 100
#define MINPOP 1
#define MAXX 5
#define MAXY 5
#define MAXGEN 1000

using namespace std;

ofstream outfile;

int translookup[512] = {0, 1, 2, 6, 1, 3, 6, 13, 2, 6, 4, 12, 5, 14, 17, 22, 51, 52, 53, 57, 52, 54, 57, 64, 53, 57, 55, 63, 56, 65, 68, 73, 2, 5, 4, 17, 6, 14, 12, 22, 7, 18, 10, 28, 18, 23, 28, 36, 53, 56, 55, 68, 57, 65, 63, 73, 58, 69, 61, 79, 69, 74, 79, 87, 1, 3, 5, 14, 8, 9, 16, 24, 6, 13, 17, 22, 16, 24, 30, 35, 52, 54, 56, 65, 59, 60, 67, 75, 57, 64, 68, 73, 67, 75, 81, 86, 5, 15, 11, 21, 16, 25, 26, 40, 18, 29, 27, 37, 31, 41, 39, 45, 56, 66, 62, 72, 67, 76, 77, 91, 69, 80, 78, 88, 82, 92, 90, 96, 2, 5, 7, 18, 5, 15, 18, 29, 4, 17, 10, 28, 11, 21, 27, 37, 53, 56, 58, 69, 56, 66, 69, 80, 55, 68, 61, 79, 62, 72, 78, 88, 4, 11, 10, 27, 17, 21, 28, 37, 10, 27, 20, 32, 27, 38, 32, 42, 55, 62, 61, 78, 68, 72, 79, 88, 61, 78, 71, 83, 78, 89, 83, 93, 6, 14, 18, 23, 16, 25, 31, 41, 12, 22, 28, 36, 26, 40, 39, 45, 57, 65, 69, 74, 67, 76, 82, 92, 63, 73, 79, 87, 77, 91, 90, 96, 17, 21, 27, 38, 30, 34, 39, 44, 28, 37, 32, 42, 39, 44, 47, 48, 68, 72, 78, 89, 81, 85, 90, 95, 79, 88, 83, 93, 90, 95, 98, 99, 1, 8, 5, 16, 3, 9, 14, 24, 5, 16, 11, 26, 15, 25, 21, 40, 52, 59, 56, 67, 54, 60, 65, 75, 56, 67, 62, 77, 66, 76, 72, 91, 6, 16, 17, 30, 13, 24, 22, 35, 18, 31, 27, 39, 29, 41, 37, 45, 57, 67, 68, 81, 64, 75, 73, 86, 69, 82, 78, 90, 80, 92, 88, 96, 3, 9, 15, 25, 9, 19, 25, 33, 14, 24, 21, 40, 25, 33, 34, 43, 54, 60, 66, 76, 60, 70, 76, 84, 65, 75, 72, 91, 76, 84, 85, 94, 14, 25, 21, 34, 24, 33, 40, 43, 23, 41, 38, 44, 41, 46, 44, 49, 65, 76, 72, 85, 75, 84, 91, 94, 74, 92, 89, 95, 92, 97, 95, 100, 6, 16, 18, 31, 14, 25, 23, 41, 17, 30, 27, 39, 21, 34, 38, 44, 57, 67, 69, 82, 65, 76, 74, 92, 68, 81, 78, 90, 72, 85, 89, 95, 12, 26, 28, 39, 22, 40, 36, 45, 28, 39, 32, 47, 37, 44, 42, 48, 63, 77, 79, 90, 73, 91, 87, 96, 79, 90, 83, 98, 88, 95, 93, 99, 13, 24, 29, 41, 24, 33, 41, 46, 22, 35, 37, 45, 40, 43, 44, 49, 64, 75, 80, 92, 75, 84, 92, 97, 73, 86, 88, 96, 91, 94, 95, 100, 22, 40, 37, 44, 35, 43, 45, 49, 36, 45, 42, 48, 45, 49, 48, 50, 73, 91, 88, 95, 86, 94, 96, 100, 87, 96, 93, 99, 96, 100, 99, 101};
const char *intlookup[51]={"0","1c","1e","2c","2e","2k","2a","2i","2n","3c","3e","3k","3a","3i","3n","3y","3q","3j","3r","4c","4e","4k","4a","4i","4n","4y","4q","4j","4r","4t","4w","4z","5c","5e","5k","5a","5i","5n","5y","5q","5j","5r","6c","6e","6k","6a","6i","6n","7c","7e","8"};

unsigned long long int hsh (int x, int y)
{
	return (x+((unsigned long long int)y<<32)+((unsigned long long int)1<<31)+((unsigned long long int)1<<63));
}

int rv(unsigned long long int hsh,bool w)
{
	if(!w) return (hsh%((unsigned long long int)1<<32)-(1<<31));
	return ((hsh>>32) - (1<<31));
}

struct mainarr
{
	//int translist[102]= {0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
	int translist[102]= {0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
	vector<unsigned long long int> clist,gbcl;
	vector<int> trgbcl;
	
	bool cellvalue (int x, int y)
	{	
		return (binary_search(clist.begin(),clist.end(),hsh(x,y)));
	}
	
	bool cellvalue (int x, int y, vector<unsigned long long int> tmp)
	{	
		for(unsigned long long int k : tmp)
		{
			if (hsh(x,y)==k) return 1;
		}
		return 0;
	}
	
	int trans(int x, int y)
	{
		int i=0,r=0;
		while (i<9)
		{
			r+=(cellvalue(x+i/3-1,y+i%3-1))<<(i++);
		}
		return translookup[r];
	}
	
	set<int> totrans()
	{
		set<int> arrout;
		int y=0;
		for(auto i : gbcl)
		{
			int q=trgbcl[y];
			if (translist[q]==2)
			{
				arrout.insert(q);
			}
			y++;
		}
		return arrout;
	}
	
	vector<unsigned long long int> evolve(int t[102])
	{
		vector<unsigned long long int> tmp;
		int y=0;
		for(unsigned long long int v : gbcl)
		{
			if(t[trgbcl[y]]==1)
			{
				tmp.push_back(v);
			}
			y++;
		}
		sort(tmp.begin(),tmp.end());
		return tmp;
	}
	
	void updategbcl()
	{
		vector<unsigned long long int> tmp;
		for (auto v : clist)
		{
			for (int j=-1;j<=1;j++)
			{
				for(int k=-1;k<=1;k++)
				{
					if(!(cellvalue(rv(v,0)+j,rv(v,1)+k,tmp))) tmp.push_back(hsh(rv(v,0)+j,rv(v,1)+k));
				}
			}
		}
		sort(tmp.begin(),tmp.end());
		vector<int> tmp2;
		gbcl=tmp;
		for (int k=0;k<gbcl.size();k++)
		{
			tmp2.push_back(trans(rv(gbcl[k],0),rv(gbcl[k],1)));
		}
		trgbcl=tmp2;
	}
};

mainarr arr[MAXGEN];
int initsymm=8;

bool compareclist(mainarr i,mainarr j)
{
	if((initsymm==1||initsymm==2||initsymm==4||initsymm==5||initsymm==6||initsymm==7) && i.clist==j.clist) return 1;
	if (i.clist.size() != j.clist.size()) return 0;
	sort(i.clist.begin(),i.clist.end());sort(j.clist.begin(),j.clist.end());
	unsigned long long int k = (i.clist[0])-(j.clist[0]);
	for(int q=1;q<i.clist.size();q++)
	{
		if(((i.clist[q])-(j.clist[q]))!=k) return 0;
	}
	return 1;
}

vector<unsigned long long int> RLEtocelllist(string RLE)
{
	vector<unsigned long long int> tmp;
	int cx=0,cy=0;
	for(int i=0;i<RLE.size();i++)
	{
		if (RLE[i]=='o')
		{
			tmp.push_back(hsh(cx,cy));
			cx++;
		}
		else if (RLE[i]=='b')
		{
			cx++;
		}
		else if (RLE[i]=='$')
		{
			cy++;
			cx=0;
		}
	}
	sort(tmp.begin(),tmp.end());
	return tmp;
}

__int128 intpow(int y)
{
	__int128 t = 1;
	return t << y;
}

void printint128(__int128 n)
{
	if (n == 0)  
		cout << '0';
	char str[40] = {0};
	char *s = str + 39;
	while (n != 0) 
	{
		*--s = "0123456789"[n % 10];
		n /= 10;
	}
	cout << s;
}

mainarr combo(mainarr curr, set<int> tlist, __int128 index)
{
	const int y = tlist.size();
	int binlist[y];
	mainarr next;
	for(int i = y-1;i>=0;i--)
	{
		binlist[i] = index%2;
		index >>= 1;
	}
	for (int i=0;i<102;i++)
	{
		if (curr.translist[i]!=2)
		{
			next.translist[i] = curr.translist[i];
		}
		else if(tlist.find(i) != tlist.end())
		{
			next.translist[i] = binlist[distance(tlist.begin(),tlist.find(i))];
		}
		else
		{
			next.translist[i] = 2;
		}
	}
	next.clist = curr.evolve(next.translist);
	return next;
}

int inc=0;
int pattcount=0;
__int128 postotalindex[MAXGEN+1][2];

int symmetry(mainarr a)
{
	if (initsymm==7)
	{
		return 7;
	}
	mainarr ar,afx,afy,afxy,afxr,afyr;
	bool arc,afxc,afyc,afxyc,afxrc,afyrc;
	switch(initsymm){
		case 6:
		{
			for(unsigned long long int i : a.clist)
			{
				ar.clist.push_back(hsh(-rv(i,1),rv(i,0)));
			}
			sort(ar.clist.begin(),ar.clist.end());
			arc=compareclist(a,ar);
			if(!arc) return 6;
			return 7;
		}	
		case 5:
		{
			for(unsigned long long int i : a.clist)
			{
				ar.clist.push_back(hsh(-rv(i,1),rv(i,0)));
			}
			sort(ar.clist.begin(),ar.clist.end());
			arc=compareclist(a,ar);
			if(!arc) return 5;
			return 7;
		}
		
		case 4:
		{
			for(unsigned long long int i : a.clist)
			{
				afx.clist.push_back(hsh(-rv(i,0),rv(i,1)));
			}
			sort(afx.clist.begin(),afx.clist.end());
			afxc=compareclist(a,afx);
			if(!afxc) return 4;
			return 7;
		}
		
		case 3:
		{
			for(unsigned long long int i : a.clist)
			{
				afxy.clist.push_back(hsh(-rv(i,0),-rv(i,1)));
				ar.clist.push_back(hsh(-rv(i,1),rv(i,0)));
			}
			sort(afxy.clist.begin(),afxy.clist.end());
			sort(ar.clist.begin(),ar.clist.end());
			afxyc=compareclist(a,afxy);
			arc=compareclist(a,ar);
			if(!afxyc) return 3;
			if(!arc) return 6;
			return 7;
		}
		
		case 2:
		{
			for(unsigned long long int i : a.clist)
			{
				afxy.clist.push_back(hsh(-rv(i,0),-rv(i,1)));
				ar.clist.push_back(hsh(-rv(i,1),rv(i,0)));
			}
			sort(afxy.clist.begin(),afxy.clist.end());
			sort(ar.clist.begin(),ar.clist.end());
			afxyc=compareclist(a,afxy);
			arc=compareclist(a,ar);
			if(!afxyc) return 2;
			if(!arc) return 5;
			return 7;
		}
		
		case 1:
		{
			for(unsigned long long int i : a.clist)
			{
				afx.clist.push_back(hsh(-rv(i,0),rv(i,1)));
				ar.clist.push_back(hsh(-rv(i,1),rv(i,0)));
				afxr.clist.push_back(hsh(rv(i,1),rv(i,0)));
			}
			sort(afx.clist.begin(),afx.clist.end());
			sort(ar.clist.begin(),ar.clist.end());
			sort(afxr.clist.begin(),afxr.clist.end());
			afxc=compareclist(a,afx);
			arc=compareclist(a,ar);
			afxrc=compareclist(a,afxr);
			if(arc) 
			{
				if (!afxc) return 4;
				return 7;
			}
			if(afxc) return 5;
			if(afxrc) return 6;
			return 1;
		}
		default:
		{
			for(unsigned long long int i : a.clist)
			{
				ar.clist.push_back(hsh(-rv(i,1),rv(i,0)));
				afx.clist.push_back(hsh(-rv(i,0),rv(i,1)));
				afy.clist.push_back(hsh(rv(i,0),-rv(i,1)));
				afxy.clist.push_back(hsh(-rv(i,0),-rv(i,1)));
				afxr.clist.push_back(hsh(rv(i,1),rv(i,0)));
				afyr.clist.push_back(hsh(-rv(i,1),-rv(i,0)));
			}
			sort(ar.clist.begin(),ar.clist.end());
			sort(afx.clist.begin(),afx.clist.end());
			sort(afy.clist.begin(),afy.clist.end());
			sort(afxy.clist.begin(),afxy.clist.end());
			sort(afxr.clist.begin(),afxr.clist.end());
			sort(afyr.clist.begin(),afyr.clist.end());
			arc=compareclist(a,ar),afxc=compareclist(a,afx),afyc=compareclist(a,afy),afxyc=compareclist(a,afxy),afxrc=compareclist(a,afxr),afyrc=compareclist(a,afyr);
			if(!(arc|afxc|afyc|afxyc|afxrc|afyrc)) return 0; //C1
			if(!arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) return 1; //C2
			if(!arc&(afxc!=afyc)&!afxyc&!afxrc&!afyrc) return 2; //D2+
			if(!arc&!afxc&!afyc&!afxyc&(afxrc!=afyrc)) return 3; //D2x
			if(arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) return 4; //C4
			if(!arc&afxc&afyc&afxyc&!afxrc&!afyrc) return 5; //D4+
			if(!arc&!afxc&!afyc&afxyc&afxrc&afyrc) return 6; //D4x
			return 7; //D8
		}
	}
}

bool checktrans(mainarr next,int n)
{
	if (next.clist.empty())
	{
		return 0;
	}
	unsigned long long int nxf = next.clist[0];
	unsigned long long int nxb = next.clist.back();
	int h = rv(nxb,1)-rv(nxf,1);
	int wmx=rv(nxb,0),wmn=rv(nxf,0);
	for(unsigned long long int i : next.clist)
	{
		if (rv(i,0)>wmx)
		{
			wmx=rv(i,0);
		}
		if (rv(i,0)<wmn)
		{
			wmn=rv(i,0);
		}
	}
	int w = wmx-wmn;
	if(w>MAXX-1) return 0;
	if(h>MAXY-1) return 0;
	if(next.clist.size()>MAXPOP || next.clist.size()<MINPOP) return 0;
	if(symmetry(next)!=initsymm) return 0;
	for(int i=0;i<n+1;i++)
	{
		if(arr[i].clist.size()>0)
		{
			if (compareclist(next,arr[i]))
			{
				if(i==0)
				{
					outfile << "B";
					for (int i=0;i<51;i++)
					{
						if (next.translist[i]==1) 
						{
							outfile << intlookup[i];
						}
					}
					outfile << "/S";
					for (int i=51;i<102;i++)
					{
						if (next.translist[i]==1)
						{
							outfile << intlookup[i%51];
						}
					}
					outfile << ": (" << rv(nxf,0)-rv(arr[i].clist[0],0) << "," << rv(nxf,1)-rv(arr[i].clist[0],1) << ")/" << n-i+1 << endl;
					pattcount++;
				}
				return 0;
			}
		}
	}
	return 1;
}

void branch(int n)
{
	set<int> trstr = arr[n].totrans();
	__int128 b = intpow(trstr.size());
	for(__int128 i=0;i<b;i++)
	{
		inc++;
		postotalindex[n][0]=i;postotalindex[n][1]=b;
		if(inc%250000==0)
		{
			cout << "Depth: " << n+1 << ", progress: ";
			for(int q=0; q<n+1;q++)
			{
				cout << "[";
				printint128(postotalindex[q][0]);
				cout << "/";
				printint128(postotalindex[q][1]);
				cout << "],";
				
			}
			cout << endl << "Patterns: " << pattcount;
			cout << endl << endl;
		}
		mainarr q = combo(arr[n],trstr,i);
		bool o = checktrans(q,n);
		if(o && n+1<MAXGEN)
		{
			arr[n+1]=q;
			arr[n+1].updategbcl();
			branch(n+1);
			arr[n+1]={0};
		}
		
		// if(MAXGEN==n+1 && o)
		// {
			// ofstream outfile2;
			// outfile2.open("supertesttree.txt",ios_base::app);
			// outfile2 << "B";
			// for (int i=0;i<51;i++)
			// {
				// if (q.translist[i]==1) 
				// {
					// outfile2 << intlookup[i];
				// }
			// }
			// outfile2 << "/S";
			// for (int i=51;i<102;i++)
			// {
				// if (q.translist[i]==1)
				// {
					// outfile2 << intlookup[i%51];
				// }
			// }
			// outfile2 << ", {";
			// for (int i=0;i<102;i++)
			// {
				// outfile2 << q.translist[i] << ",";
			// }
			// outfile2 << "}\n";
			// outfile2.close();
		// }
		
		postotalindex[n][0]=0;postotalindex[n][1]=0;
	}	
}

int main(int argc, char **argv)
{
	chrono::steady_clock::time_point t1=chrono::steady_clock::now();
	// ofstream outfile2;
	// outfile2.open("supertesttree.txt");
	// outfile2.close();
	string RLE;
	if (argc >1)
	{
		RLE = argv[1];
	}
	else
	{
		cout << "Input RLE missing"<< endl;
		return 0;
	}
	outfile.open("supertest.txt");
	outfile.close();
	arr[0].clist = RLEtocelllist(RLE);
	arr[0].updategbcl();
	initsymm=symmetry(arr[0]);
	cout << "initsymm: " << initsymm << endl;
	outfile.open("supertest.txt",ios_base::app);
	outfile << RLE << endl << endl;
	branch(0);
	outfile.close();
	cout << "time: " << chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - t1).count() << " seconds, patterns: " << pattcount << ", inc: " << inc << endl;
}
