#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <array>
#include <getopt.h>

#define MAXPOP 400
#define MINPOP 1
#define MAXX 3
#define MAXY 3
#define MAXGEN 400
#define MINGEN 1

using namespace std;

ofstream outfile;

const int translookup[512] = {0, 1, 2, 6, 1, 3, 6, 13, 2, 6, 4, 12, 5, 14, 17, 22, 51, 52, 53, 57, 52, 54, 57, 64, 53, 57, 55, 63, 56, 65, 68, 73, 2, 5, 4, 17, 6, 14, 12, 22, 7, 18, 10, 28, 18, 23, 28, 36, 53, 56, 55, 68, 57, 65, 63, 73, 58, 69, 61, 79, 69, 74, 79, 87, 1, 3, 5, 14, 8, 9, 16, 24, 6, 13, 17, 22, 16, 24, 30, 35, 52, 54, 56, 65, 59, 60, 67, 75, 57, 64, 68, 73, 67, 75, 81, 86, 5, 15, 11, 21, 16, 25, 26, 40, 18, 29, 27, 37, 31, 41, 39, 45, 56, 66, 62, 72, 67, 76, 77, 91, 69, 80, 78, 88, 82, 92, 90, 96, 2, 5, 7, 18, 5, 15, 18, 29, 4, 17, 10, 28, 11, 21, 27, 37, 53, 56, 58, 69, 56, 66, 69, 80, 55, 68, 61, 79, 62, 72, 78, 88, 4, 11, 10, 27, 17, 21, 28, 37, 10, 27, 20, 32, 27, 38, 32, 42, 55, 62, 61, 78, 68, 72, 79, 88, 61, 78, 71, 83, 78, 89, 83, 93, 6, 14, 18, 23, 16, 25, 31, 41, 12, 22, 28, 36, 26, 40, 39, 45, 57, 65, 69, 74, 67, 76, 82, 92, 63, 73, 79, 87, 77, 91, 90, 96, 17, 21, 27, 38, 30, 34, 39, 44, 28, 37, 32, 42, 39, 44, 47, 48, 68, 72, 78, 89, 81, 85, 90, 95, 79, 88, 83, 93, 90, 95, 98, 99, 1, 8, 5, 16, 3, 9, 14, 24, 5, 16, 11, 26, 15, 25, 21, 40, 52, 59, 56, 67, 54, 60, 65, 75, 56, 67, 62, 77, 66, 76, 72, 91, 6, 16, 17, 30, 13, 24, 22, 35, 18, 31, 27, 39, 29, 41, 37, 45, 57, 67, 68, 81, 64, 75, 73, 86, 69, 82, 78, 90, 80, 92, 88, 96, 3, 9, 15, 25, 9, 19, 25, 33, 14, 24, 21, 40, 25, 33, 34, 43, 54, 60, 66, 76, 60, 70, 76, 84, 65, 75, 72, 91, 76, 84, 85, 94, 14, 25, 21, 34, 24, 33, 40, 43, 23, 41, 38, 44, 41, 46, 44, 49, 65, 76, 72, 85, 75, 84, 91, 94, 74, 92, 89, 95, 92, 97, 95, 100, 6, 16, 18, 31, 14, 25, 23, 41, 17, 30, 27, 39, 21, 34, 38, 44, 57, 67, 69, 82, 65, 76, 74, 92, 68, 81, 78, 90, 72, 85, 89, 95, 12, 26, 28, 39, 22, 40, 36, 45, 28, 39, 32, 47, 37, 44, 42, 48, 63, 77, 79, 90, 73, 91, 87, 96, 79, 90, 83, 98, 88, 95, 93, 99, 13, 24, 29, 41, 24, 33, 41, 46, 22, 35, 37, 45, 40, 43, 44, 49, 64, 75, 80, 92, 75, 84, 92, 97, 73, 86, 88, 96, 91, 94, 95, 100, 22, 40, 37, 44, 35, 43, 45, 49, 36, 45, 42, 48, 45, 49, 48, 50, 73, 91, 88, 95, 86, 94, 96, 100, 87, 96, 93, 99, 96, 100, 99, 101};
const char *intlookup[51]={"0","1c","1e","2c","2e","2k","2a","2i","2n","3c","3e","3k","3a","3i","3n","3y","3q","3j","3r","4c","4e","4k","4a","4i","4n","4y","4q","4j","4r","4t","4w","4z","5c","5e","5k","5a","5i","5n","5y","5q","5j","5r","6c","6e","6k","6a","6i","6n","7c","7e","8"};

struct mainarr
{
	array<int,102> translist= {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
	array<array<int,MAXY+3>,MAXX+3> clist={0};
	vector<int> trgbcl;
	int tx,ty;
	int nx,ny;
	int p;
	int trans(int x,int y)
	{
		return translookup[
		((x==0 || y==0) ? 0:clist[x-1][y-1])+
		(y==0 ? 0:(clist[x][y-1])<<1)+
		(y==0 ? 0:(clist[x+1][y-1])<<2)+
		(x==0 ? 0:(clist[x-1][y])<<3)+
		((clist[x][y])<<4)+
		((clist[x+1][y])<<5)+
		(x==0 ? 0:(clist[x-1][y+1])<<6)+
		((clist[x][y+1])<<7)+
		((clist[x+1][y+1])<<8)];
	}
	
	vector<int> totrans()
	{
		vector<int> arrout;
		for(int i=0;i<MAXX+2;i++)
		{
			for(int j=0;j<MAXY+2;j++)
			{
				if (translist[trans(i,j)]==2) arrout.push_back(trans(i,j));
			}
		}
		sort(arrout.begin(),arrout.end());
		arrout.erase(unique(arrout.begin(),arrout.end()),arrout.end());
		return arrout;
	}
	
	void evolve(array<array<int,MAXY+3>,MAXX+3> &z, array<int,102> &t)
	{
		int mx=MAXX+4,my=MAXY+4,p=0;
		int bx=-1,by=-1;
		array<array<int,MAXY+3>,MAXX+3> tmp={0};
		for(int i=0;i<MAXX+2;i++)
		{
			for(int j=0;j<MAXY+2;j++)
			{
				if (t[trans(i,j)]==1)
				{
					if (i<mx) mx = i;
					if (j<my) my = j;
					if (i>bx) bx = i;
					if (j>by) by = j;
					p++;
					tmp[i][j]=1;
				}
			}
		}
		bx-=mx-1;
		by-=my-1;
		if (mx==MAXX+4)
		{
			tmp[0][2] = MAXX+4;
			tmp[1][0] = MAXY+4;
			tmp[2][0] = p;
			z = tmp;
		}
		if (mx==1 && my==1) 
		{
			tmp[0][0] = tx+mx-1;
			tmp[0][1] = ty+my-1;
			tmp[0][2] = bx;
			tmp[1][0] = by;
			tmp[2][0] = p;
			z = tmp;
		}
		array<array<int,MAXY+3>,MAXX+3> tmp2={0};
		for(int i=1;i<MAXX+3-mx;i++)
		{
			for(int j=1;j<MAXY+3-my;j++)
			{
				z[i][j] = tmp[i+mx-1][j+my-1];
			}
		}
		z[0][0] = tx+mx-1;
		z[0][1] = ty+my-1;
		z[0][2] = bx;
		z[1][0] = by;
		z[2][0] = p;
		//return {0};
		//z.clist = tmp2;
	}
};

mainarr arr[MAXGEN];
mainarr q;
int initsymm=8;

bool compareclist(mainarr& i,mainarr& j)
{
	return (i.clist == j.clist);
}

array<array<int,MAXY+3>,MAXX+3> RLEtocelllist(string RLE)
{
	int bx=-1,by=-1,p=0;
	int mx=MAXX+4,my=MAXY+4;
	array<array<int,MAXY+3>,MAXX+3> tmp = {0};
	int cx=1,cy=1, currn = -1;
	for(int i=0;i<RLE.size();i++)
	{
		if (RLE[i]>47 && RLE[i]<58)
		{
			if (currn==-1) currn=RLE[i]-48;
			else currn=currn*10+RLE[i]-48;
		}
		if (RLE[i]=='o')
		{
			if (currn==-1)
			{
				if(cx < MAXX+2 && cy < MAXY+2) 
				{
					if (cx<mx) mx = cx;
					if (cy<my) my = cy;
					if (cx>bx) bx = cx;
					if (cy>by) by = cy;
					p++;
					tmp[cx][cy]=1;
				}
				cx++;
			}
			else
			{
				if(cx+currn-1 < MAXX+2 && cy < MAXY+2) 
				{
					if (cx<mx) mx = cx;
					if (cy<my) my = cy;
					if (cx+currn-1>bx) bx = cx+currn-1;
					if (cy>by) by = cy;
					p+=currn;
					for(int s=0;s<currn;s++) tmp[cx++][cy]=1;
				}
			}
			currn=-1;
		}
		else if (RLE[i]=='b')
		{
			if (currn==-1) cx++;
			else cx+=currn;
			currn=-1;
		}
		else if (RLE[i]=='$')
		{
			if (currn==-1)
			{
				cy++;
				cx=1;
			}
			else
			{
				cy+=currn;
				cx=1;
			}
			currn=-1;
		}
	}
	bx-=mx-1;
	by-=my-1;
	if (mx==MAXX+4) 
	{
		tmp[1][0]=1;
		return tmp;
	}
	if (mx==1 && my==1) 
	{
		tmp[0][0] = bx;
		tmp[0][1] = by;
		tmp[2][0] = p;
		return tmp;
	}
	array<array<int,MAXY+3>,MAXX+3> tmp2={0};
	for(int i=1;i<MAXX+2-mx;i++)
	{
		for(int j=1;j<MAXY+2-my;j++)
		{
			tmp2[i][j] = tmp[i+mx-1][j+my-1];
		}
	}
	tmp2[0][0] = bx;
	tmp2[0][1] = by;
	tmp2[2][0] = p;
	return tmp2;
}

mainarr endpatt;

__int128 intpow(int y)
{
	return (__int128)1 << y;
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

void combo(mainarr& curr, vector<int>& tlist, __int128 index)
{
	q.clist = {0};
	const int y = tlist.size();
	int binlist[y];
	//mainarr next;
	for(int i = y-1;i>=0;i--)
	{
		binlist[i] = index%2;
		index >>= 1;
	}
	q.translist=curr.translist;
	for (int j=0;j<y;j++)
	{
		q.translist[tlist[j]] = binlist[j];
	}
	curr.evolve(q.clist,q.translist);
	q.tx = q.clist[0][0];
	q.ty = q.clist[0][1];
	q.nx = q.clist[0][2];
	q.ny = q.clist[1][0];
	q.p = q.clist[2][0];
	q.clist[0][0] = 0,q.clist[0][1] = 0;
	q.clist[0][2] = 0,q.clist[1][0] = 0;
	q.clist[2][0] = 0;
}

long long int inc=0;
int pattcount=0;
__int128 postotalindex[MAXGEN+1][2];

int initsymmetry(mainarr a)
{
	bool arc=1,afxc=1,afyc=1,afxyc=1,afxrc=1,afyrc=1;
	
	for(int j=1;j<a.ny+1;j++)
	{
		for(int i=1;i<a.nx+1;i++)
		{
			cout << a.clist[i][j] << "," ;
			if (a.clist[i][j]!=a.clist[a.ny-j+1][i]) arc=0;
			if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
			if (a.clist[i][j]!=a.clist[i][a.ny-j+1]) afyc=0;
			if (a.clist[i][j]!=a.clist[a.nx-i+1][a.ny-j+1]) afxyc=0;
			if (a.clist[i][j]!=a.clist[j][i]) afxrc=0;
			if (a.clist[i][j]!=a.clist[a.ny-j+1][a.nx-i+1]) afyrc=0;
		}
		cout << endl;
	}
	cout << endl;
	
	if(!(arc|afxc|afyc|afxyc|afxrc|afyrc)) return 0; //C1
	if(!arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) return 1; //C2
	if(!arc&(afxc!=afyc)&!afxyc&!afxrc&!afyrc) return 2; //D2+
	if(!arc&!afxc&!afyc&!afxyc&(afxrc!=afyrc)) return 3; //D2x
	if(arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) return 4; //C4
	if(!arc&afxc&afyc&afxyc&!afxrc&!afyrc) return 5; //D4+
	if(!arc&!afxc&!afyc&afxyc&afxrc&afyrc) return 6; //D4x
	return 7; //D8
}

bool symmetrycheck(mainarr& a)
{
	int sy=8;
	bool arc=1,afxc=1,afyc=1,afxyc=1,afxrc=1,afyrc=1;
	switch(initsymm){
		case 7: 
		{
			return 1;
		}
		case 6:
		{
			for(int j=1;j<a.ny+1;j++)
			{
				for(int i=1;i<a.nx+1;i++)
				{
					if (a.clist[i][j]!=a.clist[a.ny-j+1][i]) arc=0;
				}
			}
			if(!arc) return 1;
			return 0;
		}
		case 5:
		{
			for(int j=1;j<a.ny+1;j++)
			{
				for(int i=1;i<a.nx+1;i++)
				{
					if (a.clist[i][j]!=a.clist[a.ny-j+1][i]) arc=0;
				}
			}
			if(!arc) return 1;
			return 0;
		}
		case 4:
		{
			for(int j=1;j<a.ny+1;j++)
			{
				for(int i=1;i<a.nx+1;i++)
				{
					if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
				}
			}
			if(!afxc) return 1;
			return 0;
		}
		case 3:
		{
			for(int j=1;j<a.ny+1;j++)
			{
				for(int i=1;i<a.nx+1;i++)
				{
					if (a.clist[i][j]!=a.clist[a.nx-i+1][a.ny-j+1]) afxyc=0;
				}
			}
			if(!afxyc) return 1;
			return 0;
		}
		case 2:
		{
			for(int j=1;j<a.ny+1;j++)
			{
				for(int i=1;i<a.nx+1;i++)
				{
					if (a.clist[i][j]!=a.clist[a.nx-i+1][a.ny-j+1]) afxyc=0;
				}
			}
			if(!afxyc) return 1;
			return 0;
		}
		case 1:
		{
			for(int j=1;j<a.ny+1;j++)
			{
				for(int i=1;i<a.nx+1;i++)
				{
					if (a.clist[i][j]!=a.clist[a.ny-j+1][i]) arc=0;
					if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
					if (a.clist[i][j]!=a.clist[j][i]) afxrc=0;
				}
			}
			if(!arc&!afxc&!afxrc) return 1;
			return 0;
		}
		default:
		{
			for(int j=1;j<a.ny+1;j++)
			{
				for(int i=1;i<a.nx+1;i++)
				{
					if (a.clist[i][j]!=a.clist[a.ny-j+1][i]) arc=0;
					if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
					if (a.clist[i][j]!=a.clist[i][a.ny-j+1]) afyc=0;
					if (a.clist[i][j]!=a.clist[a.nx-i+1][a.ny-j+1]) afxyc=0;
					if (a.clist[i][j]!=a.clist[j][i]) afxrc=0;
					if (a.clist[i][j]!=a.clist[a.ny-j+1][a.nx-i+1]) afyrc=0;
				}
			}
			
			if(!(arc|afxc|afyc|afxyc|afxrc|afyrc)) return 1;
			return 0;
		}
	}
}

bool checktrans(mainarr& next,int n)
{
	int w = next.nx;
	if (w==MAXX+4) return 0;
	int h = next.ny;
	if(w>MAXX) return 0;
	if(h>MAXY) return 0;
	if(next.p<MINPOP || next.p > MAXPOP) return 0;
	if (endpatt.nx!=0 && compareclist(next,endpatt))// && next.tx==0 && next.ty==0)
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
		outfile << ": " << n+1 <<endl;
		pattcount++;
		return 0;
	}
	if(endpatt.nx==0 && !symmetrycheck(next)) return 0;
	for(int i=0;i<n+1;i++)
	{
		if(w>=0)
		{
			if (compareclist(next,arr[i]))
			{
				if(endpatt.nx==0 && i==0 && n+1 >= MINGEN)
				{
					int popmin=MAXPOP+1;
					for (int i=0;i<n+1;i++)
					{
						if (arr[i].p<popmin) popmin=arr[i].p;
					}
					
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
					outfile << ": (" << next.tx << "," << next.ty << ")/" << n+1 << ", Minpop:" << popmin << endl;
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
	vector<int> trstr = arr[n].totrans();
	__int128 b = intpow(trstr.size());
	for(__int128 i=0;i<b;i++)
	{
		inc++;
		postotalindex[n][0]=i;postotalindex[n][1]=b;
		if(inc%1000000==0)
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
		combo(arr[n],trstr,i);
		bool o = checktrans(q,n);
		if(o && n+1<MAXGEN)
		{
			arr[n+1]=q;
			branch(n+1);
		}		
		postotalindex[n][0]=0;postotalindex[n][1]=0;
	}	
}

int lookup2[234]={0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,8,0,0,0,0,0,0,0,9,10,11,12,13,14,15,16,17,18,0,0,0,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,0,0,0,42,43,44,45,46,47,0,0,0,0,0,0,0,48,49,0,0,0,0,0,0,0,0,0,0,0,50,0,0,0,0,0,0,0,0,0,0,0,0,51,0,0,0,0,0,0,0,0,0,0,0,0,52,53,0,0,0,0,0,0,0,0,0,0,0,54,55,56,57,58,59,0,0,0,0,0,0,0,60,61,62,63,64,65,66,67,68,69,0,0,0,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,0,0,0,93,94,95,96,97,98,0,0,0,0,0,0,0,99,100,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,};

void macbipartial (string partial)
{
	int bs, numflag, minusflag;
	
	int bst[18]={0};
	
	for (char c:partial)
	{
		//if (i==0 && c!='p') break;
		if (c=='B') bs=0;
		if (c=='S') bs=1;
		if (c>47 && c<58) 
		{
			numflag=c-48;
			minusflag=0;
			bst[numflag+9*bs]=1;
		}
		if (c=='-') minusflag=1;
		if (c>96 && c<123)
		{
			switch (c)
			{
				case 'c': arr[0].translist[lookup2[117*bs+13*numflag]]=1-minusflag; break;
				case 'e': arr[0].translist[lookup2[117*bs+13*numflag+1]]=1-minusflag; break;
				case 'k': arr[0].translist[lookup2[117*bs+13*numflag+2]]=1-minusflag; break;
				case 'a': arr[0].translist[lookup2[117*bs+13*numflag+3]]=1-minusflag; break;
				case 'i': arr[0].translist[lookup2[117*bs+13*numflag+4]]=1-minusflag; break;
				case 'n': arr[0].translist[lookup2[117*bs+13*numflag+5]]=1-minusflag; break;
				case 'y': arr[0].translist[lookup2[117*bs+13*numflag+6]]=1-minusflag; break;
				case 'q': arr[0].translist[lookup2[117*bs+13*numflag+7]]=1-minusflag; break;
				case 'j': arr[0].translist[lookup2[117*bs+13*numflag+8]]=1-minusflag; break;
				case 'r': arr[0].translist[lookup2[117*bs+13*numflag+9]]=1-minusflag; break;
				case 't': arr[0].translist[lookup2[117*bs+13*numflag+10]]=1-minusflag; break;
				case 'w': arr[0].translist[lookup2[117*bs+13*numflag+11]]=1-minusflag; break;
				case 'z': arr[0].translist[lookup2[117*bs+13*numflag+12]]=1-minusflag; break;
			}
		}
	}
	
	for (int i=0;i<18;i++)
	{
		if(bst[i]==0)
		{
			for (int j=0;j<13;j++) arr[0].translist[lookup2[13*i+j]]=0;
		}
	}
}

int main(int argc, char **argv)
{
	chrono::steady_clock::time_point t1=chrono::steady_clock::now();
	// ofstream outfile2;
	// outfile2.open("supertesttree.txt");
	// outfile2.close();
	
	int z, outflag=0,minmaxflag=0;
	string file;
	string RLE;
	string minrule="B/S";
	string maxrule="B012345678/S012345678";
	
	struct option long_options[] =
	{
		{"file", 1,0,'f'},
		{"prule", 1,0,'r'},
		{"min", 1,0,'q'},
		{"max", 1,0,'s'},
		{"target", 1,0,'t'},
	};
	
	int option_index=0;
	
	while ((z=getopt_long(argc,argv,"o:p:t:r:q:s:h",long_options,&option_index))!=-1)
	{
		switch(z)
		{
			case 'p':
			{
				RLE = optarg;
				break;
			}
			case 'f':
			{
				outflag=1;
				file = optarg;
				break;
			}
			
			case 't':
			{
				endpatt.clist = RLEtocelllist(optarg);
				endpatt.nx=endpatt.clist[0][0],endpatt.ny=endpatt.clist[0][1];
				endpatt.clist[0][0]=0,endpatt.clist[0][1]=0,endpatt.clist[2][0]=0;
				break;
			}
			
			case 'r':
			{
				macbipartial(optarg);
				break;
			}
			
			case 'q':
			{
				minrule=optarg;
				minmaxflag=1;
				break;
			}
			
			case 's':
			{
				maxrule=optarg;
				minmaxflag=1;
				break;
			}
			
			case 'h':
			{
				cout
				<< "\n-p is for setting the pattern, use a headerless RLE for this in quotes, i.e. -p '3o$bo$5bo'.\n\n"
				<< "-f or --file is for setting the output file of the results.\n\n"
				<< "-t or --target is for setting an optional target pattern that is not the original set pattern,\nin the same format as -p.\n\n"
				<< "-r or --prule is for setting rulerange with Macbi partial rule format. Do not use with -q or -s.\n\n"
				<< "-q or --min is for setting rulerange minrule. Do not use with -r.\n\n"
				<< "-s or --max is for setting rulerange maxrule. Do not use with -r.\n\n"
				<< "There are 6 other constants to change at the top of the file, which set maximum population,\nminimum population, "
				<< "maximum horizontal bounding box, maximum vertical bounding box,\nmaximum generation depth, and minimum generation depth to report respectively.\n"
				<< "These must be set in the .cpp file, and it must be compiled again if these are changed.\n\n";
				exit(0);

			}
		}
	}
	
	if(minmaxflag)
	{
		int k=0, bs, numflag, minusflag, prevnum;
		int minl[102]={0};
		int maxl[102]={0};
		for (char c:minrule)
		{
			if (c=='B') {bs=0; prevnum=0;}
			if (c=='S') {bs=1; prevnum=0;}
			if (c>46 && c<58) 
			{
				if(prevnum)
				{
					for (int i=0;i<13;i++) minl[lookup2[117*bs+13*numflag+i]]=1;
				}
				numflag=c-48;
				if (k==minrule.length()-1)
				{
					for (int i=0;i<13;i++) minl[lookup2[117*bs+13*numflag+i]]=1;
				}
				prevnum=1;
				minusflag=0;
			}
			if (c=='-') 
			{
				for (int j=0;j<13;j++) minl[lookup2[117*bs+13*numflag+j]]=1;
				minusflag=1;
				prevnum=0;
			}
			if (c>96 && c<123)
			{
				prevnum=0;
				switch (c)
				{
					case 'c': minl[lookup2[117*bs+13*numflag]]=1-minusflag; break;
					case 'e': minl[lookup2[117*bs+13*numflag+1]]=1-minusflag; break;
					case 'k': minl[lookup2[117*bs+13*numflag+2]]=1-minusflag; break;
					case 'a': minl[lookup2[117*bs+13*numflag+3]]=1-minusflag; break;
					case 'i': minl[lookup2[117*bs+13*numflag+4]]=1-minusflag; break;
					case 'n': minl[lookup2[117*bs+13*numflag+5]]=1-minusflag; break;
					case 'y': minl[lookup2[117*bs+13*numflag+6]]=1-minusflag; break;
					case 'q': minl[lookup2[117*bs+13*numflag+7]]=1-minusflag; break;
					case 'j': minl[lookup2[117*bs+13*numflag+8]]=1-minusflag; break;
					case 'r': minl[lookup2[117*bs+13*numflag+9]]=1-minusflag; break;
					case 't': minl[lookup2[117*bs+13*numflag+10]]=1-minusflag; break;
					case 'w': minl[lookup2[117*bs+13*numflag+11]]=1-minusflag; break;
					case 'z': minl[lookup2[117*bs+13*numflag+12]]=1-minusflag; break;
				}
			}
			k++;
		}
		bs=0,k=0;
		for (char c:maxrule)
		{
			if (c=='B') {bs=0; prevnum=0;}
			if (c=='S') {bs=1; prevnum=0;}
			if (c>46 && c<58) 
			{
				//cout << maxrule.length() << endl;
				if(prevnum)
				{
					for (int i=0;i<13;i++) maxl[lookup2[117*bs+13*numflag+i]]=1;
				}
				numflag=c-48;
				if (k==maxrule.length()-1)
				{
					for (int i=0;i<13;i++) maxl[lookup2[117*bs+13*numflag+i]]=1;
				}
				prevnum=1;
				minusflag=0;
			}
			if (c=='-') 
			{
				for (int j=0;j<13;j++) maxl[lookup2[117*bs+13*numflag+j]]=1;
				minusflag=1;
				prevnum=0;
			}
			if (c>96 && c<123)
			{
				prevnum=0;
				switch (c)
				{
					case 'c': maxl[lookup2[117*bs+13*numflag]]=1-minusflag; break;
					case 'e': maxl[lookup2[117*bs+13*numflag+1]]=1-minusflag; break;
					case 'k': maxl[lookup2[117*bs+13*numflag+2]]=1-minusflag; break;
					case 'a': maxl[lookup2[117*bs+13*numflag+3]]=1-minusflag; break;
					case 'i': maxl[lookup2[117*bs+13*numflag+4]]=1-minusflag; break;
					case 'n': maxl[lookup2[117*bs+13*numflag+5]]=1-minusflag; break;
					case 'y': maxl[lookup2[117*bs+13*numflag+6]]=1-minusflag; break;
					case 'q': maxl[lookup2[117*bs+13*numflag+7]]=1-minusflag; break;
					case 'j': maxl[lookup2[117*bs+13*numflag+8]]=1-minusflag; break;
					case 'r': maxl[lookup2[117*bs+13*numflag+9]]=1-minusflag; break;
					case 't': maxl[lookup2[117*bs+13*numflag+10]]=1-minusflag; break;
					case 'w': maxl[lookup2[117*bs+13*numflag+11]]=1-minusflag; break;
					case 'z': maxl[lookup2[117*bs+13*numflag+12]]=1-minusflag; break;
				}
			}
			k++;
		}
		
		for(int i=0;i<102;i++)
		{
			if (minl[i]==maxl[i]) arr[0].translist[i]=minl[i];
		}
		
		arr[0].translist[0]=0;
		
		// for(int i:arr[0].translist) cout << i;
		// cout << endl;
		// for(int i:minl) cout << i;
		// cout << endl;
		// for(int i:maxl) cout << i;
		// cout << endl;
	}
	
	if(!outflag) file = "Output - EnumPattEvo.txt";
	
	outfile.open(file);
	outfile.close();
	
	arr[0].clist = RLEtocelllist(RLE);
	if (arr[0].clist[1][0])
	{
		cout << "Null RLE input, cancelling." << endl;
		return 0;
	}
	
	arr[0].tx=0,arr[0].ty=0;
	arr[0].nx=arr[0].clist[0][0],arr[0].ny=arr[0].clist[0][1];
	arr[0].p = arr[0].clist[2][0];
	arr[0].clist[0][0]=0,arr[0].clist[0][1]=0,arr[0].clist[2][0]=0;
	initsymm=initsymmetry(arr[0]);
	//cout << arr[0].nx << " " << arr[0].ny << " " << arr[0].p << " " << endl;
	cout << "initsymm: " << initsymm << endl << endl;
	outfile.open(file,ios_base::app);
	outfile << RLE << endl << endl;
	branch(0);
	outfile.close();
	cout << "time: " << chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - t1).count() << " seconds, patterns: " << pattcount << ", inc: " << inc << endl;
}
