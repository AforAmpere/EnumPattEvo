#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <array>
#include <getopt.h>
#include <cmath>

#define PRUNE (next.p>10000)
#define SPECIAL_RESULT (1==2)
#define MAXX 5
#define MAXY 5
#define MAXGEN 20000
#define MINGEN 1

//Define to allow basically infinite generation depth with a slight speed sacrifice, unless the bounding box is huge, where it may still crash if you don't have the RAM.
#define NORECURSE

using namespace std;

ofstream outfile;
ofstream backupfile;
ifstream loadfile;

const int translookup[512] = {0, 1, 2, 6, 1, 3, 6, 13, 2, 6, 4, 12, 5, 14, 17, 22, 51, 52, 53, 57, 52, 54, 57, 64, 53, 57, 55, 63, 56, 65, 68, 73, 2, 5, 4, 17, 6, 14, 12, 22, 7, 18, 10, 28, 18, 23, 28, 36, 53, 56, 55, 68, 57, 65, 63, 73, 58, 69, 61, 79, 69, 74, 79, 87, 1, 3, 5, 14, 8, 9, 16, 24, 6, 13, 17, 22, 16, 24, 30, 35, 52, 54, 56, 65, 59, 60, 67, 75, 57, 64, 68, 73, 67, 75, 81, 86, 5, 15, 11, 21, 16, 25, 26, 40, 18, 29, 27, 37, 31, 41, 39, 45, 56, 66, 62, 72, 67, 76, 77, 91, 69, 80, 78, 88, 82, 92, 90, 96, 2, 5, 7, 18, 5, 15, 18, 29, 4, 17, 10, 28, 11, 21, 27, 37, 53, 56, 58, 69, 56, 66, 69, 80, 55, 68, 61, 79, 62, 72, 78, 88, 4, 11, 10, 27, 17, 21, 28, 37, 10, 27, 20, 32, 27, 38, 32, 42, 55, 62, 61, 78, 68, 72, 79, 88, 61, 78, 71, 83, 78, 89, 83, 93, 6, 14, 18, 23, 16, 25, 31, 41, 12, 22, 28, 36, 26, 40, 39, 45, 57, 65, 69, 74, 67, 76, 82, 92, 63, 73, 79, 87, 77, 91, 90, 96, 17, 21, 27, 38, 30, 34, 39, 44, 28, 37, 32, 42, 39, 44, 47, 48, 68, 72, 78, 89, 81, 85, 90, 95, 79, 88, 83, 93, 90, 95, 98, 99, 1, 8, 5, 16, 3, 9, 14, 24, 5, 16, 11, 26, 15, 25, 21, 40, 52, 59, 56, 67, 54, 60, 65, 75, 56, 67, 62, 77, 66, 76, 72, 91, 6, 16, 17, 30, 13, 24, 22, 35, 18, 31, 27, 39, 29, 41, 37, 45, 57, 67, 68, 81, 64, 75, 73, 86, 69, 82, 78, 90, 80, 92, 88, 96, 3, 9, 15, 25, 9, 19, 25, 33, 14, 24, 21, 40, 25, 33, 34, 43, 54, 60, 66, 76, 60, 70, 76, 84, 65, 75, 72, 91, 76, 84, 85, 94, 14, 25, 21, 34, 24, 33, 40, 43, 23, 41, 38, 44, 41, 46, 44, 49, 65, 76, 72, 85, 75, 84, 91, 94, 74, 92, 89, 95, 92, 97, 95, 100, 6, 16, 18, 31, 14, 25, 23, 41, 17, 30, 27, 39, 21, 34, 38, 44, 57, 67, 69, 82, 65, 76, 74, 92, 68, 81, 78, 90, 72, 85, 89, 95, 12, 26, 28, 39, 22, 40, 36, 45, 28, 39, 32, 47, 37, 44, 42, 48, 63, 77, 79, 90, 73, 91, 87, 96, 79, 90, 83, 98, 88, 95, 93, 99, 13, 24, 29, 41, 24, 33, 41, 46, 22, 35, 37, 45, 40, 43, 44, 49, 64, 75, 80, 92, 75, 84, 92, 97, 73, 86, 88, 96, 91, 94, 95, 100, 22, 40, 37, 44, 35, 43, 45, 49, 36, 45, 42, 48, 45, 49, 48, 50, 73, 91, 88, 95, 86, 94, 96, 100, 87, 96, 93, 99, 96, 100, 99, 101};
const char *intlookup[51]={"0","1c","1e","2c","2e","2k","2a","2i","2n","3c","3e","3k","3a","3i","3n","3y","3q","3j","3r","4c","4e","4k","4a","4i","4n","4y","4q","4j","4r","4t","4w","4z","5c","5e","5k","5a","5i","5n","5y","5q","5j","5r","6c","6e","6k","6a","6i","6n","7c","7e","8"};

int initsymm=8;
int endsymm=8;
//Vertical, horizontal, left diag, right diag = 1,2,3,4
int vhlr=0;
int endvhlr=0;

struct mainarr
{
	array<int,102> translist= {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
	array<array<int,MAXY+3>,MAXX+3> clist={0}, trarr={0};
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
	
	array<int,103> totrans()
	{
		array<int, 103> arrout;
		int q=0,tr=0;
		int d[102]={0};
		switch(initsymm)
		{
			case 7:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=i;j<(ny+3)/2;j++)
					{
						q=trans(i,j);
						trarr[i][j]=q;
						if (translist[q]==2&&d[q]==0) 
						{
							arrout[tr]=q;
							d[q]=1;
							tr++;
						}
					}
				}
				break;
			}
			case 6:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=i;j<ny+2-i;j++)
					{
						q=trans(i,j);
						trarr[i][j]=q;
						if (translist[q]==2&&d[q]==0) 
						{
							arrout[tr]=q;
							d[q]=1;
							tr++;
						}
					}
				}
				break;
			}
			case 5:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=0;j<(ny+3)/2;j++)
					{
						q=trans(i,j);
						trarr[i][j]=q;
						if (translist[q]==2&&d[q]==0) 
						{
							arrout[tr]=q;
							d[q]=1;
							tr++;
						}
					}
				}
				break;
			}
			case 4:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=0;j<(ny+3)/2;j++)
					{
						q=trans(i,j);
						trarr[i][j]=q;
						if (translist[q]==2&&d[q]==0) 
						{
							arrout[tr]=q;
							d[q]=1;
							tr++;
						}
					}
				}
				break;
			}
			case 3:
			{
				if(vhlr==3)
				{
					for(int i=0;i<nx+2;i++)
					{
						for(int j=i;j<ny+2;j++)
						{
							q=trans(i,j);
							trarr[i][j]=q;
							if (translist[q]==2&&d[q]==0) 
							{
								arrout[tr]=q;
								d[q]=1;
								tr++;
							}
						}
					}
				}
				else
				{
					for(int i=0;i<nx+2;i++)
					{
						for(int j=0;j<ny+2-i;j++)
						{
							q=trans(i,j);
							trarr[i][j]=q;
							if (translist[q]==2&&d[q]==0) 
							{
								arrout[tr]=q;
								d[q]=1;
								tr++;
							}
						}
					}
				}
				break;
			}
			case 2:
			{
				if(vhlr==1)
				{
					for(int i=0;i<(nx+3)/2;i++)
					{
						for(int j=0;j<ny+2;j++)
						{
							q=trans(i,j);
							trarr[i][j]=q;
							if (translist[q]==2&&d[q]==0) 
							{
								arrout[tr]=q;
								d[q]=1;
								tr++;
							}
						}
					}
				}
				else
				{
					for(int i=0;i<nx+2;i++)
					{
						for(int j=0;j<(ny+3)/2;j++)
						{
							q=trans(i,j);
							trarr[i][j]=q;
							if (translist[q]==2&&d[q]==0) 
							{
								arrout[tr]=q;
								d[q]=1;
								tr++;
							}
						}
					}
				}
				break;
			}
			case 1:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=0;j<ny+2;j++)
					{
						q=trans(i,j);
						trarr[i][j]=q;
						if (translist[q]==2&&d[q]==0) 
						{
							arrout[tr]=q;
							d[q]=1;
							tr++;
						}
					}
				}
				break;
			}
			default:
			{
				for(int i=0;i<nx+2;i++)
				{
					for(int j=0;j<ny+2;j++)
					{
						q=trans(i,j);
						trarr[i][j]=q;
						if (translist[q]==2&&d[q]==0) 
						{
							arrout[tr]=q;
							d[q]=1;
							tr++;
						}
					}
				}
				break;
			}
		}
		arrout[102]=tr;
		sort(arrout.begin(),arrout.begin()+tr);
		return arrout;
	}
	
	void evolve(array<array<int,MAXY+3>,MAXX+3> &z, array<int,102> &t)
	{
		int mx=MAXX+4,my=MAXY+4,p=0;
		int bx=-1,by=-1;
		array<array<int,MAXY+3>,MAXX+3> tmp={0};
		switch(initsymm)
		{
			case 7:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=i;j<(ny+3)/2;j++)
					{
						if (t[trarr[i][j]]==1)
						{
							if (i<mx) mx = i;
							// a=(i==j);
							// b=(j==ny+1-j);
							if(!(i==j)&&!(j==ny+1-j)) 
							{
								p+=8;
								tmp[i][j]=1;
								tmp[j][i]=1;
								tmp[nx+1-i][j]=1;
								tmp[j][nx+1-i]=1;
								tmp[nx+1-i][ny+1-j]=1;
								tmp[ny+1-j][nx+1-i]=1;
								tmp[i][ny+1-j]=1;
								tmp[ny+1-j][i]=1;
							}
							if((i==j)&&!(j==ny+1-j)) 
							{
								p+=4;
								tmp[i][j]=1;
								tmp[nx+1-i][j]=1;
								tmp[nx+1-i][ny+1-j]=1;
								tmp[i][ny+1-j]=1;
							}
							if(!(i==j)&&(j==ny+1-j)) 
							{
								p+=4;
								tmp[i][j]=1;
								tmp[j][i]=1;
								tmp[nx+1-i][ny+1-j]=1;
								tmp[ny+1-j][nx+1-i]=1;
							}
							if((i==j)&&(j==ny+1-j)) 
							{
								p+=1;
								tmp[i][j]=1;
							}
						}
					}
				}
				my=mx;
				bx=nx-mx+1;
				by=bx;
				break;
			}
			case 6:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=i;j<ny+2-i;j++)
					{
						if (t[trarr[i][j]]==1)
						{
							if (i<mx) mx = i;
							// a=(i==j);
							// b=(i==ny+1-j);
							if(!(i==j)&&!(i==ny+1-j))
							{								
								p+=4;
								tmp[i][j]=1;
								tmp[j][i]=1;
								tmp[nx+1-i][ny+1-j]=1;
								tmp[ny+1-j][nx+1-i]=1;
							}
							if((i==j)!=(i==ny+1-j)) 
							{
								p+=2;
								tmp[i][j]=1;
								tmp[nx+1-i][ny+1-j]=1;
							}
							if((i==j)&&(i==ny+1-j))
							{								
								p+=1;
								tmp[i][j]=1;
							}
						}
					}
				}
				my=mx;
				bx=nx-mx+1;
				by=bx;
				break;
			}
			case 5:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=0;j<(ny+3)/2;j++)
					{
						if (t[trarr[i][j]]==1)
						{
							if (i<mx) mx = i;
							if (j<my) my = j;
							if(!(i==nx+1-i)&&!(j==ny+1-j)) 
							{
								p+=4;
								tmp[i][j]=1;
								tmp[nx+1-i][j]=1;
								tmp[nx+1-i][ny+1-j]=1;
								tmp[i][ny+1-j]=1;
							}
							if((i==nx+1-i)!=(j==ny+1-j)) 
							{
								p+=2;
								tmp[i][j]=1;
								tmp[nx+1-i][ny+1-j]=1;
							}
							if((i==nx+1-i)&&(j==ny+1-j)) 
							{
								tmp[i][j]=1;
								p+=1;
							}
						}
					}
				}
				bx=nx-mx+1;
				by=ny-my+1;
				break;
			}
			case 4:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=0;j<(ny+2)/2;j++)
					{
						if (t[trarr[i][j]]==1)
						{
							if (i<mx) mx = i;
							if (j<mx) mx = j;
							p+=4;
							tmp[i][j]=1;
							tmp[j][nx+1-i]=1;
							tmp[nx+1-i][ny+1-j]=1;
							tmp[ny+1-j][i]=1;
						}
					}
				}
				if(nx%2==1&&t[trarr[(nx+1)/2][(ny+1)/2]]==1)
				{
					if ((nx+1)/2<mx) mx = (nx+1)/2;
					if ((ny+1)/2<my) my = (ny+1)/2;
					tmp[(nx+1)/2][(ny+1)/2]=1;
					p+=1;
				}
				my=mx;
				bx=nx-mx+1;
				by=bx;
				break;
			}
			case 3:
			{
				if(vhlr==3)
				{
					for(int i=0;i<nx+2;i++)
					{
						for(int j=i;j<ny+2;j++)
						{
							if (t[trarr[i][j]]==1)
							{
								if (i<mx) mx = i;
								if (j>by) by = j;
								//a=(i!=j);
								if(i!=j) 
								{
									p+=2;
									tmp[i][j]=1;
									tmp[j][i]=1;
								}
								else 
								{
									p+=1;
									tmp[i][j]=1;
								}
							}
						}
					}
					my=mx;
					bx=by;
				}
				else
				{
					for(int i=0;i<nx+2;i++)
					{
						for(int j=0;j<ny+2-i;j++)
						{
							if (t[trarr[i][j]]==1)
							{
								if (i<mx) mx = i;
								if (j<my) my = j;
								//a=(i!=ny+1-j);
								if(i!=ny+1-j) 
								{
									p+=2;
									tmp[i][j]=1;
									tmp[ny+1-j][nx+1-i]=1;
								}
								else 
								{
									p+=1;
									tmp[i][j]=1;
								}
							}
						}
					}
					by=nx-mx+1;
					bx=ny-my+1;
				}
				break;
			}
			case 2:
			{
				if(vhlr==1)
				{
					for(int i=0;i<(nx+3)/2;i++)
					{
						for(int j=0;j<ny+2;j++)
						{
							if (t[trarr[i][j]]==1)
							{
								if (i<mx) mx = i;
								if (j<my) my = j;
								if (j>by) by = j;
								//a=(i!=nx+1-i);
								if(i!=nx+1-i) 
								{
									p+=2;
									tmp[i][j]=1;
									tmp[nx+1-i][j]=1;
								}
								else 
								{
									p+=1;
									tmp[i][j]=1;
								}
							}
						}
					}
					bx=nx-mx+1;
				}
				else
				{
					for(int i=0;i<nx+2;i++)
					{
						for(int j=0;j<(ny+3)/2;j++)
						{
							if (t[trarr[i][j]]==1)
							{
								if (i<mx) mx = i;
								if (i>bx) bx = i;
								if (j<my) my = j;
								//a=(j!=ny+1-j);
								if(j!=ny+1-j) 
								{
									p+=2;
									tmp[i][j]=1;
									tmp[i][ny+1-j]=1;
								}
								else 
								{
									p+=1;
									tmp[i][j]=1;
								}
							}
						}
					}
					by=ny-my+1;
				}
				break;
			}
			case 1:
			{
				for(int i=0;i<(nx+3)/2;i++)
				{
					for(int j=0;j<ny+2;j++)
					{
						if (t[trarr[i][j]]==1)
						{
							if (i<mx) mx = i;
							if (j<my) my = j;
							if (ny+1-j<my) my = ny+1-j;
							//a=(i!=nx+1-i);
							if(i!=nx+1-i) 
							{
								p+=2;
								tmp[i][j]=1;
								tmp[nx+1-i][ny+1-j]=1;
							}
							else
							{								
								p+=1;
								tmp[i][j]=1;
							}
						}
					}
				}
				by=ny+1-my;
				bx=nx-mx+1;
				break;
			}
			case 0:
			{
				for(int i=0;i<nx+2;i++)
				{
					for(int j=0;j<ny+2;j++)
					{
						//if (trans(i,j)) j=j;
						if (t[trarr[i][j]]==1)
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
				break;
			}
			default:
			{
				for(int i=0;i<nx+2;i++)
				{
					for(int j=0;j<ny+2;j++)
					{
						//if (trans(i,j)) j=j;
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
				break;
			}
		}
		
		bx-=mx-1;
		by-=my-1;
		//cout<<"bx:"<<bx<<",by:"<<by<<endl;
		if (mx==MAXX+4)
		{
			z[0][2] = MAXX+4;
			z[1][0] = MAXY+4;
			z[2][0] = p;
			return;
		}
		if (mx==1 && my==1) 
		{
			for(int i=1;i<bx+1;i++)
			{
				for(int j=1;j<by+1;j++)
				{
					z[i][j] = tmp[i][j];
				}
			}
			z[0][0] = tx+mx-1;
			z[0][1] = ty+my-1;
			z[0][2] = bx;
			z[1][0] = by;
			z[2][0] = p;
			return;
		}
		if (mx!=1 || my!=1)
		{
			for(int i=1;i<bx+1;i++)
			{
				for(int j=1;j<by+1;j++)
				{
					z[i][j] = tmp[i+mx-1][j+my-1];
				}
			}
			z[0][0] = tx+mx-1;
			z[0][1] = ty+my-1;
			z[0][2] = bx;
			z[1][0] = by;
			z[2][0] = p;
			return;
		}
	}
};

struct int128
{
	__int128 i;
	
};

#ifndef NORECURSE
mainarr arr[MAXGEN];
#endif
#ifdef NORECURSE

mainarr* arr = new mainarr[MAXGEN];

#endif
mainarr q;
int xdis=100000;
int ydis=100000;
bool gxflag=0;
bool lxflag=0;
bool gyflag=0;
bool lyflag=0;
bool axflag=0;
bool ayflag=0;
bool moveflag=0;
bool explodeflag=1;
bool evolveflag=0;
bool emptyflag=0;
bool s5flag=0;
bool appendflag=0;
__int128 totrules=0;
int b1efrontend=0;
int b2afrontend=0;
int b1e2cfrontend=0;
int b2a2cfrontend=0;
int b2a3ifrontend=0;
int commonconst=0;
string backuploc="";
string loadloc="";
vector<array<int,4>> speeds(0);

array<array<int,MAXY+3>,MAXX+3> RLEtocelllist(string RLE)
{
	int bx=-1,by=-1,p=0;
	int mx=MAXX+4,my=MAXY+4;
	array<array<int,MAXY+3>,MAXX+3> tmp = {0};
	int cx=1,cy=1, currn = -1;
	for(unsigned int i=0;i<RLE.size();i++)
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

void printint128(__int128 n, ofstream& o)
{
	if (n == 0)  
		o << '0';
	char str[40] = {0};
	char *s = str + 39;
	while (n != 0) 
	{
		*--s = "0123456789"[n % 10];
		n /= 10;
	}
	o << s;
}

template<typename T>
string clisttoRLE(T& inparr, int nx, int ny)
{
	string RLE;
	int runningo=0,runningb=0,runningd=0;
	for(int j=1;j<ny+1;j++)
	{
		for(int i=1;i<nx+1;i++)
		{
			if(inparr[i][j]==1)
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
			if(inparr[i][j]==0)
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
		if(runningb<nx+1)
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

void canonize(mainarr& inparr,array<array<int,MAXY+3>,MAXX+3>& outclist,int flip)
{
	switch(flip)
	{
		case 0:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[i][j]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 1:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[inparr.nx+1-i][j]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 2:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[i][inparr.ny+1-j]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 3:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[inparr.nx+1-i][inparr.ny+1-j]=inparr.clist[i][j];
				}
			}
			break;
		}
		#if MAXX==MAXY
		case 4:
		{

			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[j][i]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 5:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[j][inparr.nx-i+1]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 6:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[inparr.ny-j+1][i]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 7:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[inparr.ny+1-j][inparr.nx+1-i]=inparr.clist[i][j];
				}
			}
			break;
		}
		#endif
	}
}

#if MAXX!=MAXY
void canonize(mainarr& inparr,array<array<int,MAXX+3>,MAXY+3>& outclist,int flip)
{
	switch(flip)
	{
		case 4:
		{

			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[j][i]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 5:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[j][inparr.nx-i+1]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 6:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[inparr.ny-j+1][i]=inparr.clist[i][j];
				}
			}
			break;
		}
		case 7:
		{
			for(int j=1;j<inparr.ny+1;j++)
			{
				for(int i=1;i<inparr.nx+1;i++)
				{
					if(inparr.clist[i][j]) outclist[inparr.ny+1-j][inparr.nx+1-i]=inparr.clist[i][j];
				}
			}
			break;
		}
	}
}
#endif

void combo(mainarr& curr, array<int,103>& tlist, __int128 index)
{
	q.clist = {0};
	const int y = tlist[102];
	int binlist[y];
	//mainarr next;
	for(int i = 0;i<y;i++)
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

bool compareclist(mainarr& i,mainarr& j)
{
	if(i.p!=j.p) return 0;
	if(i.nx!=j.nx) return 0;
	if(i.ny!=j.ny) return 0;
	return (i.clist == j.clist);
}

bool compareclist(mainarr& comparr,string j,int minx=0, int miny=0, int maxx=0, int maxy=0)
{
	if(maxx==0||maxx>comparr.nx-1) maxx = comparr.nx-1;
	if(maxy==0||maxy>comparr.ny-1) maxy = comparr.ny-1;
	if(minx<0) minx=0;
	if(miny<0) miny=0;
	array<array<int,MAXY+3>,MAXX+3> comp = RLEtocelllist(j);
	if(comp[2][0]==0)
	{
		for(int i=minx+1;i<maxx+2;i++)
		{
			for(int j=miny+1;j<maxy+2;j++)
			{
				if(comparr.clist[i][j])
				{
					return 0;
				}
			}
		}
		return 1;
	}
	if(maxx<minx||maxy<miny) return 0;
	comp[0][0]=0;
	comp[0][1]=0;
	comp[2][0]=0;
	if(minx==0&&miny==0&&maxx==comparr.nx-1&&maxy==comparr.ny-1) return (comparr.clist == comp);
	array<array<int,MAXY+3>,MAXX+3> newi={0};
	int tx=0,ty=0;
	for(int i=minx+1;i<maxx+2&&tx==0;i++)
	{
		for(int j=miny+1;j<maxy+2&&tx==0;j++)
		{
			if(comparr.clist[i][j])
			{
				tx=i;
			}
		}
	}
	for(int j=miny+1;j<maxy+2&&ty==0;j++)
	{
		for(int i=minx+1;i<maxx+2&&ty==0;i++)
		{
			if(comparr.clist[i][j])
			{
				ty=j;
			}
		}
	}
	for(int i=tx;i<maxx+2;i++)
	{
		for(int j=ty;j<maxy+2;j++)
		{
			if(comparr.clist[i][j]==1) newi[i-tx+1][j-ty+1]=1;
		}
	}
	return (newi == comp);
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
			cout << a.clist[i][j] << " " ;
			if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && i<a.ny+1)?a.clist.at(a.ny-j+1)[i]:0)) arc=0;
			if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
			if (a.clist[i][j]!=a.clist[i][a.ny-j+1]) afyc=0;
			if (a.clist[i][j]!=a.clist[a.nx-i+1][a.ny-j+1]) afxyc=0;
			if (a.clist[i][j]!=((j<a.nx+1 && i<a.ny+1)?a.clist[j][i]:0)) afxrc=0;
			if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && a.nx-i+1<a.ny+1)?a.clist[a.ny-j+1][a.nx-i+1]:0)) afyrc=0;
		}
		cout << endl;
	}
	cout << endl;
	
	if(!(arc|afxc|afyc|afxyc|afxrc|afyrc)) return 0; //C1
	if(!arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) return 1; //C2
	if(!arc&(afxc!=afyc)&!afxyc&!afxrc&!afyrc) 
	{
		if (afxc) return 10;
		else return 18;
		//return 2; //D2+
	}
	if(!arc&!afxc&!afyc&!afxyc&(afxrc!=afyrc)) 
	{
		if (afxrc) return 27;
		else return 35;
		//return 3; //D2x
	}
	if(arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) return 4; //C4
	if(!arc&afxc&afyc&afxyc&!afxrc&!afyrc) return 5; //D4+
	if(!arc&!afxc&!afyc&afxyc&afxrc&afyrc) return 6; //D4x
	return 7; //D8
}

bool symmetrycheck(mainarr& a)
{
	bool arc=1,afxc=1,afyc=1,afxyc=1,afxrc=1,afyrc=1;
	switch(initsymm){
		case 7: 
		{
			return 1;
		}
		case 6:
		{
			for(int i=1;i<(a.nx+3)/2;i++)
			{
				for(int j=i;j<a.ny+1-i;j++)
				{
					if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && i<a.ny+1)?a.clist.at(a.ny-j+1)[i]:0)) return 1;
				}
			}
			return 0;
		}
		case 5:
		{
			if (a.nx!=a.ny) return 1;
			for(int j=1;j<(a.ny+3)/2;j++)
			{
				for(int i=1;i<(a.nx+3)/2;i++)
				{
					if (a.clist[i][j]!=((j<a.nx+1 && i<a.ny+1)?a.clist[j][i]:0)) return 1;
				}
			}
			return 0;
		}
		case 4:
		{
			for(int j=1;j<(a.ny+3)/2;j++)
			{
				for(int i=1;i<(a.nx+3)/2;i++)
				{
					if (a.clist[i][j]!=((j<a.nx+1 && i<a.ny+1)?a.clist[j][i]:0)) return 1;
				}
			}
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
					if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && i<a.ny+1)?a.clist.at(a.ny-j+1)[i]:0)) arc=0;
					if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
					if (a.clist[i][j]!=((j<a.nx+1 && i<a.ny+1)?a.clist[j][i]:0)) afxrc=0;
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
					if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && i<a.ny+1)?a.clist.at(a.ny-j+1)[i]:0)) arc=0;
					if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
					if (a.clist[i][j]!=a.clist[i][a.ny-j+1]) afyc=0;
					if (a.clist[i][j]!=a.clist[a.nx-i+1][a.ny-j+1]) afxyc=0;
					if (a.clist[i][j]!=((j<a.nx+1 && i<a.ny+1)?a.clist[j][i]:0)) afxrc=0;
					if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && a.nx-i+1<a.ny+1)?a.clist[a.ny-j+1][a.nx-i+1]:0)) afyrc=0;
				}
			}
			
			if(!(arc|afxc|afyc|afxyc|afxrc|afyrc)) return 1;
			return 0;
		}
	}
}

bool symmetrycheck2(mainarr& a)
{
	int s=-1,v=0;
	bool arc=1,afxc=1,afyc=1,afxyc=1,afxrc=1,afyrc=1;
	
	for(int j=1;j<a.ny+1;j++)
	{
		for(int i=1;i<a.nx+1;i++)
		{
			if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && i<a.ny+1)?a.clist.at(a.ny-j+1)[i]:0)) arc=0;
			if (a.clist[i][j]!=a.clist[a.nx-i+1][j]) afxc=0;
			if (a.clist[i][j]!=a.clist[i][a.ny-j+1]) afyc=0;
			if (a.clist[i][j]!=a.clist[a.nx-i+1][a.ny-j+1]) afxyc=0;
			if (a.clist[i][j]!=((j<a.nx+1 && i<a.ny+1)?a.clist[j][i]:0)) afxrc=0;
			if (a.clist[i][j]!=((a.ny-j+1<a.nx+1 && a.nx-i+1<a.ny+1)?a.clist[a.ny-j+1][a.nx-i+1]:0)) afyrc=0;
		}
	}
	while(1)
	{
		if(!(arc|afxc|afyc|afxyc|afxrc|afyrc)) {s=0;break;} //C1
		if(!arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) {s=1;break;} //C2
		if(!arc&(afxc!=afyc)&!afxyc&!afxrc&!afyrc) 
		{
			if (afxc) v=1;
			else v=2;
			s=2; //D2+
			break;
		}
		if(!arc&!afxc&!afyc&!afxyc&(afxrc!=afyrc)) 
		{
			if (afxrc) v=3;
			else v=4;
			s=3; //D2x
			break;
		}
		if(arc&!afxc&!afyc&afxyc&!afxrc&!afyrc) {s=4;break;} //C4
		if(!arc&afxc&afyc&afxyc&!afxrc&!afyrc) {s=5;break;} //D4+
		if(!arc&!afxc&!afyc&afxyc&afxrc&afyrc) {s=6;break;} //D4x
		s=7;break; //D8
	}
	
	if
	(
	(s==endsymm&&v!=endvhlr)||
	(s==7&&endsymm!=7)||
	(s==6&&endsymm!=6&&endsymm!=7)||
	(s==5&&endsymm!=5&&endsymm!=7)||
	(s==4&&endsymm!=4&&endsymm!=7)||
	(s==3&&endsymm!=3&&endsymm!=6&&endsymm!=7)||
	(s==2&&endsymm!=2&&endsymm!=5&&endsymm!=7)||
	(s==1&&(endsymm==0||endsymm==2||endsymm==3))||
	(s==7&&a.nx%2!=endpatt.nx%2)||
	(s==6&&a.nx%2!=endpatt.nx%2)||
	(s==5&&(a.nx%2!=endpatt.nx%2||a.ny%2!=endpatt.ny%2))||
	(s==4&&a.nx%2!=endpatt.nx%2)||
	(s==2&&(v==1&&a.nx%2!=endpatt.nx%2))||
	(s==2&&(v==2&&a.ny%2!=endpatt.ny%2))||
	(s==1&&(a.nx%2!=endpatt.nx%2||a.ny%2!=endpatt.ny%2))
	)
	{
		return 0;
	}
	return 1;
}

int checkfrontend(mainarr& next, int initi)
{
	int t=1,r=1,b=1,l=1,b1e=0,b2c=0,b2a=0,b3i=0;
	int dir=0;
	int q=0;
	
	if (initi==0)
	{
		b1e=next.translist[2]%2;
		if(next.translist[3]==0) b2c=0;
		else b2c=1;
		b2a=next.translist[6]%2;
		b3i=next.translist[13]%2;
	}
	
	else
	{
		if (initi%2==1) b1e=1;
		if (initi%4>>1==1) b2c=1;
		if (initi%8>>2==1) b2a=1;
		if (initi>>3==1) b3i=1;
	}
	
	if (b1e&&!b2c) dir=b1efrontend;
	if (b1e&&b2c) dir=b1e2cfrontend;
	if (b2a&&!b2c) dir=b2afrontend;
	if (b2a&&b2c) dir=b2a2cfrontend;
	if (b2a&&b3i) dir=b2a3ifrontend;
	
	if ((dir%2)==1||((initsymm==7||initsymm==6||initsymm==4||(initsymm==3&&vhlr==3))&&!initi)) t=0;
	if ((dir%4)>>1==1||((initsymm!=0&&!(initsymm==2&&vhlr==2))&&!initi)) r=0;
	if ((dir%8)>>2==1||((initsymm!=0&&!(initsymm==2&&vhlr==1)&&!(initsymm==3&&vhlr==3))&&!initi)) b=0;
	if ((dir>>3)==1) l=0;
	
	//cout << b1e << b2c << b2a << b3i << initi << endl;
	
	if(b1e+b2a==0) return 1;
	if(b1e+b2a==2) 
	{
		//cout << "Found" << endl;
		return 0;
	}
	
	if (t)
	{
		int offrun=100000,onrun=0,ch=0;
		for (int j=1; j<next.nx+1;j++)
		{
			if (next.clist[j][1]==0) 
			{
				if (next.clist[j-1][1]==1) 
				{
					if (onrun==(b1e?1:2) && ch==1) 
					{
						ch=2;
					}
					else ch=0;
					onrun=0;
				}
				offrun+=1;
				if (ch==2 && offrun>(b2c)) ch=3;
			}
			if (next.clist[j][1]==1)
			{
				if (onrun>(b1e?0:1)) ch=0;
				if (next.clist[j-1][1]==0)
				{
					if (offrun>(b2c)) ch=1;
					else ch=0;
					offrun=0;
				}
				onrun+=1;
			}
			if (j==next.nx && (ch==1 && onrun==(b1e?1:2) || ch==2)) ch=3;
			if (b2a && b3i && onrun>1) ch=3;
			if (ch==3) j=next.nx+1;
			//cout << ch << " ";
		}
		//cout << endl;
		if (ch==3)
		{
			if (initi==0) return 0;
			else q++;
		}
		//else cout << "Not" << endl;
	}
	if (r)
	{
		int offrun=10000,onrun=0,ch=0;
		for (int j=1; j<next.ny+1;j++)
		{
			if (next.clist[next.nx][j]==0) 
			{
				if (next.clist[next.nx][j-1]==1) 
				{
					if (onrun==(b1e?1:2) && ch==1) 
					{
						ch=2;
					}
					else ch=0;
					onrun=0;
				}
				offrun+=1;
				if (ch==2 && offrun>(b2c)) ch=3;
			}
			if (next.clist[next.nx][j]==1)
			{
				if (onrun>(b1e?0:1)) ch=0;
				if (next.clist[next.nx][j-1]==0)
				{
					if (offrun>(b2c)) ch=1;
					else ch=0;
					offrun=0;
				}
				onrun+=1;
			}
			if (j==next.ny && (ch==1 && onrun==(b1e?1:2) || ch==2)) ch=3;
			if (b2a && b3i && onrun>1) ch=3;
			if (ch==3) j=next.ny+1;
			//cout << ch << " ";
		}
		if (ch==3)
		{
			if (initi==0) return 0;
			else q+=2;
		}
	}
	if (b)
	{
		int offrun=10000,onrun=0,ch=0;
		for (int j=1; j<next.nx+1;j++)
		{
			if (next.clist[j][next.ny]==0) 
			{
				if (next.clist[j-1][next.ny]==1) 
				{
					if (onrun==(b1e?1:2) && ch==1) 
					{
						ch=2;
					}
					else ch=0;
					onrun=0;
				}
				offrun+=1;
				if (ch==2 && offrun>(b2c)) ch=3;
			}
			if (next.clist[j][next.ny]==1)
			{
				if (onrun>(b1e?0:1)) ch=0;
				if (next.clist[j-1][next.ny]==0)
				{
					if (offrun>(b2c)) ch=1;
					else ch=0;
					offrun=0;
				}
				onrun+=1;
			}
			if (j==next.nx && (ch==1 && onrun==(b1e?1:2) || ch==2)) ch=3;
			if (b2a && b3i && onrun>1) ch=3;
			if (ch==3) j=next.nx+1;
			//cout << ch << " ";
		}
		if (ch==3)
		{
			if (initi==0) return 0;
			else q+=4;
		}
	}
	if (l)
	{
		int offrun=10000,onrun=0,ch=0;
		for (int j=1; j<next.ny+1;j++)
		{
			if (next.clist[1][j]==0) 
			{
				if (next.clist[1][j-1]==1) 
				{
					if (onrun==(b1e?1:2) && ch==1) 
					{
						ch=2;
					}
					else ch=0;
					onrun=0;
				}
				offrun+=1;
				if (ch==2 && offrun>(b2c)) ch=3;
			}
			if (next.clist[1][j]==1)
			{
				if (onrun>(b1e?0:1)) ch=0;
				if (next.clist[1][j-1]==0)
				{
					if (offrun>(b2c)) ch=1;
					else ch=0;
					offrun=0;
				}
				onrun+=1;
			}
			if (j==next.ny && (ch==1 && onrun==(b1e?1:2) || ch==2)) ch=3;
			if (b2a && b3i && onrun>1) ch=3;
			if (ch==3) j=next.ny+1;
			//cout << ch << " ";
		}
		if (ch==3)
		{
			if (initi==0) return 0;
			else q+=8;
		}
	}
	if (initi==0) return 1;
	return q;
}

bool checktrans(mainarr& next,int n)
{	
	if (next.p==0)
	{
		if (emptyflag)
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
					outfile << intlookup[i-51];
				}
			}
			outfile << ": " << n+1 << endl;
			pattcount++;
		}
		return 0;
	}
	
	int w = next.nx;
	int h = next.ny;
	if(w>MAXX) 
	{
		return 0;
	}
	if(h>MAXY)
	{
		return 0;
	}
	if(PRUNE) return 0;
	
	if(SPECIAL_RESULT)
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
		outfile << ": " << n+1 << endl;
		pattcount++;
		return 0;
	}
	
	int poss=0;
	for (int k=0;k<102;k++)
	{
		if (next.translist[k]==2) poss++;
	}
	if (poss<commonconst) return 0;
	if(explodeflag&&!checkfrontend(next,0)) 
	{
		return 0;
	}
	if(explodeflag&& endpatt.nx==0 && (next.translist[2]==1 && next.translist[6]==1)) return 0;
	if(endpatt.nx!=0 && !symmetrycheck2(next)) return 0;
	if (endpatt.nx!=0 && compareclist(next,endpatt) && n+1 >= MINGEN)
	{
		if(xdis!=100000 && ((!gxflag && !lxflag && ((axflag)==(next.tx==xdis))) || (gxflag && !lxflag && next.tx<=xdis) || (lxflag && !gxflag && next.tx>=xdis))) return 0;
		if(ydis!=100000 && ((!gyflag && !lyflag && ((ayflag)==(next.ty==ydis))) || (gyflag && !lyflag && next.ty<=ydis) || (lyflag && !gyflag && next.ty>=ydis))) return 0;
		if(moveflag && (next.tx==0 && next.ty==0)) return 0;
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
		outfile << ": " << n+1 << endl;
		pattcount++;
		return 0;
	}
	if((endpatt.nx==0 && !emptyflag) && !symmetrycheck(next)) return 0;
	for(int i=0;i<n+1;i++)
	{
		if(w>=0)
		{
			if (compareclist(next,arr[i]))
			{
				if(xdis!=100000 && ((!gxflag && !lxflag && ((axflag)==(next.tx==xdis))) || (gxflag && !lxflag && next.tx<=xdis) || (lxflag && !gxflag && next.tx>=xdis))) return 0;
				if(ydis!=100000 && ((!gyflag && !lyflag && ((ayflag)==(next.ty==ydis))) || (gyflag && !lyflag && next.ty<=ydis) || (lyflag && !gyflag && next.ty>=ydis))) return 0;
				if(moveflag && (next.tx-arr[i].tx==0 && next.ty-arr[i].ty==0)) return 0;
				if((endpatt.nx==0 && !emptyflag) && (i==0 || evolveflag) && n-i+1 >= MINGEN)
				{
 					int popmin=100000;
					int popminloc=i;
					for (int k=i;k<n+1;k++)
					{
						if (arr[k].p<popmin) 
						{
							popmin=arr[k].p;
							if(s5flag)
								popminloc=k;
						}
					}
					if(!s5flag)
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
						outfile << ": (" << next.tx-arr[i].tx << "," << next.ty-arr[i].ty << ")/" << n-i+1 << ", Minpop:" << popmin << ", 2^"<< poss << endl;
						pattcount++;
					}
					else
					{
						int x1=next.tx-arr[i].tx,y1=next.ty-arr[i].ty,flip=0;
						if(x1<0)
							flip++;
						if(y1<0)
							flip+=2;
						if(abs(x1)<abs(y1))
							flip+=4;
						bool flg=1;
						for(int k=0;k<speeds.size();k++)
						{
							// for(auto s:speeds[k])
								// cout<<s<<",";
							// cout<<endl;
							if(speeds[k][0]==(flip/4?abs(y1):abs(x1)) && speeds[k][1]==(flip/4?abs(x1):abs(y1)) && speeds[k][2]==n-i+1)
							{
								if(popmin>=speeds[k][3])
									flg=0;
								else
									speeds.erase(speeds.begin()+k);
								
							}
						}
						if(flg)
						{
							speeds.push_back({flip/4?abs(y1):abs(x1),flip/4?abs(x1):abs(y1),n-i+1,popmin});
							int nx,ny;
							array<array<int,MAXX+3>,MAXY+3> tmp={0};
							array<array<int,MAXY+3>,MAXX+3> tmp2={0};
							if(flip/4)
							{
								canonize(arr[popminloc],tmp,flip);
								ny=arr[popminloc].nx;
								nx=arr[popminloc].ny;
							}
							else
							{
								canonize(arr[popminloc],tmp2,flip);
								nx=arr[popminloc].nx;
								ny=arr[popminloc].ny;
							}
							outfile << popmin << ", ";
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
							if(flip/4)
								outfile<<", "<<(flip/4?abs(y1):abs(x1))<<", "<<(flip/4?abs(x1):abs(y1))<<", "<<n-i+1<<", "<<clisttoRLE(tmp,nx,ny)<<endl;
							else
								outfile<<", "<<(flip/4?abs(y1):abs(x1))<<", "<<(flip/4?abs(x1):abs(y1))<<", "<<n-i+1<<", "<<clisttoRLE(tmp2,nx,ny)<<endl;
							pattcount++;
						}
					}
				}
				return 0;
			}

		}
		else return 0;
	}
	//if(PRUNE) return 0;
	return 1;
}

int nflag=0;

void save(int n)
{
	backupfile.open(backuploc);
	for(int i=0;i<n+1;i++)
	{
		printint128(postotalindex[i][0],backupfile);
		backupfile<<endl;
	}
	backupfile.close();
}

int load()
{
	string tmp;
	int i=0;
	loadfile.open(loadloc);
	while(getline(loadfile,tmp))
	{
		postotalindex[i][0]=(__int128)stoll(tmp);
		//printint128(postotalindex[i][0]);
		//cout<<endl;
		i++;
	}
	// for(int j=0;j<i/2+1;j++)
	// {
		// combo(arr[j],trstr,postotalindex[j][0]);
		// arr[j+1]=q;
	// }
	return i-1;
}

void branch(int n)
{
	static int s=-1;
	if(n==0&&loadloc!="")
	{
		s = load();
	}
	
	array<int,103> trstr = arr[n].totrans();
	
	__int128 b = intpow(trstr[102]);
	for(__int128 i=0;i<b &&!(nflag>0 && pattcount>=nflag);i++)
	{
		if(n<=s)
		{
			i=postotalindex[n][0];
			if(n==s)
			{
				s=-1;
			}
		}
		if (nflag>0 && pattcount>=nflag) break;
		if(n>=s)
			inc++;
		postotalindex[n][0]=i;postotalindex[n][1]=b;
		if(inc%5000000==0)
		{
			if(backuploc!="")
				save(n);
			cout << "Depth: " << n+1 << ", progress: ";
			for(int j=0; j<n+1;j++)
			{
				cout << "[";
				printint128(postotalindex[j][0]);
				cout << "/";
				printint128(postotalindex[j][1]);
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
	}	
}

int lookup2[234]={0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,8,0,0,0,0,0,0,0,9,10,11,12,13,14,15,16,17,18,0,0,0,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,0,0,0,42,43,44,45,46,47,0,0,0,0,0,0,0,48,49,0,0,0,0,0,0,0,0,0,0,0,50,0,0,0,0,0,0,0,0,0,0,0,0,51,0,0,0,0,0,0,0,0,0,0,0,0,52,53,0,0,0,0,0,0,0,0,0,0,0,54,55,56,57,58,59,0,0,0,0,0,0,0,60,61,62,63,64,65,66,67,68,69,0,0,0,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,0,0,0,93,94,95,96,97,98,0,0,0,0,0,0,0,99,100,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,};

void macbipartial (string partial)
{
	int bs=-1, numflag=0, minusflag=0;
	
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
	//cout<<(int)sizeof(mainarr)*MAXGEN<<endl;
	// #ifdef NORECURSE
	// mainarr* arr = new mainarr[MAXGEN];
	// #endif
	chrono::steady_clock::time_point t1=chrono::steady_clock::now();
	
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
		{"help", 1,0,'h'},
		{"patt", 1,0,'p'},
		{"pattern", 1,0,'p'},
		{"num", 1,0,'n'},
		{"evo", 1,0,'e'},
		{"no_exp", 0,0,'z'},
		{"comm", 1,0,'c'},
		{"xtrans", 1,0,'x'},
		{"ytrans", 1,0,'y'},
		{"infile", 1,0,'i'},
		{"move",0,0,'m'},
		{"5s",0,0,'@'},
		{"backup",1,0,'b'},
		{"load",1,0,'l'},
		{"append",0,0,'a'}
	};
	
	int option_index=0;
	
	while ((z=getopt_long(argc,argv,":o:p:t:r:q:s:h:x:y:n:f:ezc:mi:@b:l:a",long_options,&option_index))!=-1)
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
				endpatt.nx=endpatt.clist[0][0],endpatt.ny=endpatt.clist[0][1],endpatt.p=endpatt.clist[2][0];
				if(endpatt.nx==0) emptyflag=1;
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
			
			case 'x':
			{
				string ts=optarg;
				if(ts[0]=='>')
				{
					gxflag=1;
					xdis=stoi(ts.substr(1,ts.length()-1));
					break;
				}
				if(ts[0]=='<')
				{
					lxflag=1;
					xdis=stoi(ts.substr(1,ts.length()-1));
					break;
				}
				if(ts[0]=='!')
				{
					axflag=1;
					xdis=stoi(ts.substr(1,ts.length()-1));
					break;
				}
				xdis=stoi(ts);
				break;
			}
			
			case 'y':
			{
				string ts=optarg;
				if(ts[0]=='>')
				{
					gyflag=1;
					ydis=stoi(ts.substr(1,ts.length()-1));
					break;
				}
				if(ts[0]=='<')
				{
					lyflag=1;
					ydis=stoi(ts.substr(1,ts.length()-1));
					break;
				}
				if(ts[0]=='!')
				{
					ayflag=1;
					ydis=stoi(ts.substr(1,ts.length()-1));
					break;
				}
				ydis=stoi(ts);
				break;
			}
			
			case 'z':
			{
				explodeflag=0;
				break;
			}
			
			case 'e':
			{
				evolveflag=1;
				break;
			}
			
			case 'c':
			{
				commonconst=atoi(optarg);
				break;
			}
			
			case 'm':
			{
				moveflag=1;
				break;
			}
			
			case 'h':
			{
				string ts=optarg;
				if(ts=="h" || ts=="help")
				{
					cout << "You've clearly figured it out." << endl;
					exit(0);
				}
				
				if(ts=="p" || ts=="patt" || ts=="pattern")
				{
					cout << "Enter the headerless RLE for the desired starting pattern in in quotes after the flag, i.e. -p '3o$bbo$bo!'." << endl;
					exit(0);
				}
				
				if(ts=="f" || ts=="file")
				{
					cout << "Enter the name of an output file for results from EPE, i.e -f Output.txt." << endl;
					exit(0);
				}
				
				if(ts=="a" || ts=="append")
				{
					cout << "Takes no arguments, makes EPE append to end of specified results file instead of clearing it." << endl;
					exit(0);
				}
				
				if(ts=="t" || ts=="target")
				{
					cout << "Enter a secondary RLE for a target pattern to have the one in -p evolve to instead, in the same format as -p. So, to look for evolutions"
					<<" from 3o! to bo$3o$bo!, do -p '3o!' -t 'bo$3o$bo!'."<< endl;
					exit(0);
				}
				
				if(ts=="r" || ts=="prule")
				{
					cout << "Input a partial rule denoting the rulerange in Macbi partial rule format in which you want to search. This is the same format used in LLS."
					<< " Do not use in conjunction with -q (--min) or -s (--max)."<< endl;
					exit(0);
				}
				
				if(ts=="q" || ts=="min")
				{
					cout << "Input the minimum rule in the desired rulerange to search in. Do not use in conjunction with -r (--prule)"<< endl;
					exit(0);
				}
				
				if(ts=="s" || ts=="max")
				{
					cout << "Input the maximum rule in the desired rulerange to search in. Do not use in conjunction with -r (--prule)"<< endl;
					exit(0);
				}
				
				if(ts=="x" || ts=="xtrans")
				{
					cout << "Input the desired bound on the translation of the top left of the bounding box of the pattern in x (where rightwards is positive). "
					<< "An optional '>', '<', or '!' can be included to set more loose bounds. For example, if the desired translation is more than 5 cells to the right "
					<< "-x '>5' would specify that."<< endl;
					
					exit(0);
				}
				
				if(ts=="y" || ts=="ytrans")
				{
					cout << "Input the desired bound on the translation of the top left of the bounding box of the pattern in y (where downwards is positive). "
					<< "An optional '>', '<', or '!' can be included to set more loose bounds. For example, if the desired translation is more than 5 cells downward "
					<< "-y '>5' would specify that."<< endl;
					
					exit(0);
				}
				
				if(ts=="n" || ts=="num")
				{
					cout << "Input the maximum number of results to find before quitting. -n 3 will find 3 results and exit, for example."<< endl;
					exit(0);
				}
				
				if(ts=="z" || ts=="no_exp")
				{
					cout << "Takes no arguments and turns off explosion detection if used."<< endl;
					exit(0);
				}
				
				if(ts=="e" || ts=="evo")
				{
					cout << "Takes no arguments and turns on reporting of stable ships or oscillators that result from the evolution of the initial pattern, but "
					<< "do not contain the initial pattern as a phase. Using this with -t is useless."<< endl;
					exit(0);
				}
				
				if(ts=="c" || ts=="comm")
				{
					cout << "Input the desired degree of commonness of the pattern in the specified rulespace. -c 8 will force any results to occur within 2^8 rules "
					<< "or more in the rulespace provided with -q, -r, and/or -s."<< endl;
					exit(0);
				}
				
				if(ts=="m" || ts=="move")
				{
					cout << "Takes no arguments. Set to make all returned patterns have moved from their initial position (with the position as the top left corner "<< endl;
					exit(0);
				}
				
				if(ts=="i" || ts=="infile")
				{
					cout << "Takes an input file as an argument. This reads a generation count and an RLE in Golly format from a file, and sets the searched pattern"<<
					"normally set by -p to said RLE, with the rulespace set to be the one where the pattern matches the behavior in the given rule for the amount"
					<<"of generations specified at the top of the file. An example of the format is as follows:"<< endl<<endl;
					cout<< "20"<<endl;
					cout<< "x = 3, y = 4, rule = B3/S23"<<endl;
					cout<< "2o$b2o$2o$o!"<<endl<<endl;
					cout<<"This will search the pattern 2o$b2o$2o$o! in the rulespace where it evolves the same as it does in B3/S23 for the first 20 generations."<<endl;
					exit(0);
				}
				
				if(ts=="5s")
				{
					cout << "Takes no arguments. Puts EPE in 5S mode."<< endl;
					exit(0);
				}
				
				if(ts=="b" || ts=="backup")
				{
					cout << "Takes an backup file as an argument. Backs up program state to the file periodically."<< endl;
					exit(0);
				}
				
				if(ts=="l" || ts=="load")
				{
					cout << "Takes a file to load from (likely a file generated from -b) as an argument. Loads program to the state saved. Make sure to"<<
					"use (mostly) the same parameters as were used in the original search, such as pattern and rulespace. Others may be able to be changed"<<
					"without breaking things." << endl;
					exit(0);
				}
				
				cout << "Command not found." << endl;
				exit(0);


			}
			
			case 'n':
			{
				nflag=atoi(optarg);
				break;
			}
			
			case 'i':
			{
				ifstream infile(optarg);
				if(!infile.is_open())
				{
					cout<<"File not found. Exiting."<<endl;
					exit(0);
				}
				int run;
				infile>>run;
				char buff=0;
				int xflag=-1, yflag=-1;
				while(buff!='B')
				{
					if(buff=='x')
					{
						while(buff!='=')
						{
							infile>>buff;
						}
						infile>>xflag;
						if(xflag>MAXX) 
						{
							cout<<"The specified x width of "<<xflag<<"is too large for the set MAXX, please change MAXX to fit."<<endl;
							exit(0);
						}
					}
					if(buff=='y')
					{
						while(buff!='=')
						{
							infile>>buff;
						}
						infile>>yflag;
						if(yflag>MAXY) 
						{
							cout<<"The specified y width of "<<yflag<<"is too large for the set MAXY, please change MAXY to fit."<<endl;
							exit(0);
						}
					}
					infile>>buff;
				}
				string strbuff;
				getline(infile,strbuff);
				strbuff+=" ";
				//cout<<strbuff<<endl;
				int k=0, bs=-1, numflag=-1, minusflag=-1, prevnum=-1;
				bs=0; prevnum=0;
				mainarr incoming;
				for(int i=0;i<102;i++)
				{
					incoming.translist[i]=0;
				}
				for (char c:strbuff)
				{
					if (c=='S' || c=='s') {bs=1; prevnum=0;}
					if (c>46 && c<58 || c==' ') 
					{
						if(prevnum)
						{
							for (int i=0;i<13;i++) incoming.translist[lookup2[117*bs+13*numflag+i]]=1;
						}
						numflag=c-48;
						if (k==strbuff.length()-2)
						{
							for (int i=0;i<13;i++) incoming.translist[lookup2[117*bs+13*numflag+i]]=1;
						}
						prevnum=1;
						minusflag=0;
					}
					if (c=='-') 
					{
						for (int j=0;j<13;j++) incoming.translist[lookup2[117*bs+13*numflag+j]]=1;
						minusflag=1;
						prevnum=0;
					}
					if (c>96 && c<123)
					{
						prevnum=0;
						switch (c)
						{
							case 'c': incoming.translist[lookup2[117*bs+13*numflag]]=1-minusflag; break;
							case 'e': incoming.translist[lookup2[117*bs+13*numflag+1]]=1-minusflag; break;
							case 'k': incoming.translist[lookup2[117*bs+13*numflag+2]]=1-minusflag; break;
							case 'a': incoming.translist[lookup2[117*bs+13*numflag+3]]=1-minusflag; break;
							case 'i': incoming.translist[lookup2[117*bs+13*numflag+4]]=1-minusflag; break;
							case 'n': incoming.translist[lookup2[117*bs+13*numflag+5]]=1-minusflag; break;
							case 'y': incoming.translist[lookup2[117*bs+13*numflag+6]]=1-minusflag; break;
							case 'q': incoming.translist[lookup2[117*bs+13*numflag+7]]=1-minusflag; break;
							case 'j': incoming.translist[lookup2[117*bs+13*numflag+8]]=1-minusflag; break;
							case 'r': incoming.translist[lookup2[117*bs+13*numflag+9]]=1-minusflag; break;
							case 't': incoming.translist[lookup2[117*bs+13*numflag+10]]=1-minusflag; break;
							case 'w': incoming.translist[lookup2[117*bs+13*numflag+11]]=1-minusflag; break;
							case 'z': incoming.translist[lookup2[117*bs+13*numflag+12]]=1-minusflag; break;
						}
					}
					k++;
				}
				incoming.translist[0]=0;
				
				// int zz=0;
				// for(auto c:incoming.translist)
				// {
					// if(c)
					// {
						// cout<<(zz/51?"s":"b")<<(intlookup[zz%51])<<",";
					// }
					// zz++;
				// }
				// cout<<endl;
				
				string rlebuff;
				while(infile>>buff)
				{
					if(buff!='\r' && buff!='\n') rlebuff+=buff;
				}
				incoming.clist=RLEtocelllist(rlebuff);
				incoming.tx=0,incoming.ty=0;
				incoming.nx=incoming.clist[0][0],incoming.ny=incoming.clist[0][1];
				incoming.p = incoming.clist[2][0];
				incoming.clist[0][0]=0,incoming.clist[0][1]=0,incoming.clist[2][0]=0;
				initsymm=0;
				array<int,103> trstr;
				array<int,102> tmp;
				while(run>0)
				{
					tmp=incoming.translist;
					for(int i=1;i<102;i++)
					{
						incoming.translist[i]=2;
					}
					trstr = incoming.totrans();
					incoming.translist=tmp;
					for(int i=0;i<trstr[102];i++)
					{
						arr[0].translist[trstr[i]]=incoming.translist[trstr[i]];
					}
					q.clist={0};
					incoming.evolve(q.clist,incoming.translist);
					incoming.clist=q.clist;
					incoming.tx = incoming.clist[0][0];
					incoming.ty = incoming.clist[0][1];
					incoming.nx = incoming.clist[0][2];
					incoming.ny = incoming.clist[1][0];
					incoming.p = incoming.clist[2][0];
					incoming.clist[0][0] = 0,incoming.clist[0][1] = 0;
					incoming.clist[0][2] = 0,incoming.clist[1][0] = 0;
					incoming.clist[2][0] = 0;
					run--;
				}
				//for(int q:incoming.translist) cout<< q<<",";
				//cout<<endl;
				initsymm=8;
				RLE = rlebuff;
				break;
			}
			
			case '@':
			{
				//moveflag=1;
				s5flag=1;
				break;
			}
			
			case 'b':
			{
				backuploc=optarg;
				break;
			}
			
			case 'l':
			{
				loadloc=optarg;
				ifstream loadfile(loadloc);
				if(!loadfile.is_open())
				{
					cout<<"Load file not found. Exiting."<<endl;
					exit(0);
				}
				break;
			}
			
			case 'a':
			{
				appendflag=1;
				break;
			}
			
			case ':':
			{
				if (optopt=='h')
				{
					cout
					<<"\n-h or --help pulls up this message. Putting a hyphenless argument after -h will give more info on\n"
					<<"it.\n\n"
					<< "-p is for setting the pattern, use a headerless RLE for this in quotes, i.e. -p '3o$bo$5bo'.\n\n"
					<< "-f or --file is for setting the output file of the results.\n\n"
					<< "-a is for allowing EPE to append results to an existing file\n\n"
					<< "-t or --target is for setting an optional target pattern that is not the original set pattern, in\n"
				    <<"the same format as -p.\n\n"
					<< "-r or --prule is for setting rulerange with Macbi partial rule format. Do not use with -q or -s.\n\n"
					<< "-q or --min is for setting rulerange minrule. Do not use with -r.\n\n"
					<< "-s or --max is for setting rulerange maxrule. Do not use with -r.\n\n"
					<< "-x or --xtrans is for setting the bounds on the translation of the bounding box in x.\n\n"
					<< "-y or --ytrans is for setting the bounds on the translation of the bounding box in y.\n\n"
					<< "-n or --num is for setting the number of results to quit when reached.\n\n"
					<< "-z or --no_exp is for turning off explosion detection.\n\n"
					<< "-e or --evo is for turning on additional reporting of results that result from the evolution of\n"
					<<"the starting pattern, but do not include it.\n\n"
					<< "-c or --comm is for setting the minimum commonness of a pattern, -c 'n' forces the pattern to work\n"
					<<"in at least 2^n rules in the given space.\n\n"
					<< "-m or --move is for turning on checking if the pattern moves from its intitial position.\n\n"
					<< "-i or --infile is for specifying an RLE (in Golly format, without comments), and a generation count\n"
					<<"to match the given pattern in the given rule. See -h i for more details\n\n"
					<< "--5s to go into 5s mode.\n\n"
					<< "-b or --backup backs up to the file specified.\n\n"
					<< "-l or --load loads from the file specified. Make sure to input the same search parameters.\n\n"
					<< "There are 6 other defines to change at the top of the file, which are for special pruning and reporting\n"
					<<"conditions, maximum horizontal bounding box, maximum vertical bounding box, maximum generation depth, and\n"
					<<"minimum generation depth to report, respectively.\n"
					<<"These must be set in the .cpp file, and it must be compiled again if these are changed.\n\n";
					exit(0);
				}
			}
		}
	}
	
	if(minmaxflag)
	{
		int k=0, bs=-1, numflag=1, minusflag=-1, prevnum=-1;
		int minl[102]={0};
		int maxl[102]={0};
		for (char c:minrule)
		{
			if (c=='B' || c=='b') {bs=0; prevnum=0;}
			if (c=='S' || c=='s') {bs=1; prevnum=0;}
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
			if (c=='B' || c=='b') {bs=0; prevnum=0;}
			if (c=='S' || c=='s') {bs=1; prevnum=0;}
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
	}
	
	if(!outflag) file = "Output - EnumPattEvo.txt";
	
	if(!appendflag)
	{
		outfile.open(file);
		outfile.close();
	}
	if(backuploc!="")
	{
		backupfile.open(backuploc);
		backupfile.close();
	}
	//for(int c:arr[0].translist) cout<<c<<",";
	//cout<<endl;
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
	int x=initsymmetry(arr[0]);
	initsymm=x%8;
	vhlr=x/8;
	if (endpatt.nx!=0)
	{
		cout << "|\nv\n\n";
	
		int y=initsymmetry(endpatt);
		endsymm=y%8;
		endvhlr=y/8;
		if
		(
		(initsymm==endsymm&&vhlr!=endvhlr)||
		(initsymm==7&&endsymm!=7)||
		(initsymm==6&&endsymm!=6&&endsymm!=7)||
		(initsymm==5&&endsymm!=5&&endsymm!=7)||
		(initsymm==4&&endsymm!=4&&endsymm!=7)||
		(initsymm==3&&endsymm!=3&&endsymm!=6&&endsymm!=7)||
		(initsymm==2&&endsymm!=2&&endsymm!=5&&endsymm!=7)||
		(initsymm==1&&(endsymm==0||endsymm==2||endsymm==3))||
		(initsymm==7&&arr[0].nx%2!=endpatt.nx%2)||
		(initsymm==6&&arr[0].nx%2!=endpatt.nx%2)||
		(initsymm==5&&(arr[0].nx%2!=endpatt.nx%2||arr[0].ny%2!=endpatt.ny%2))||
		(initsymm==4&&arr[0].nx%2!=endpatt.nx%2)||
		(initsymm==2&&(vhlr==1&&arr[0].nx%2!=endpatt.nx%2))||
		(initsymm==2&&(vhlr==2&&arr[0].ny%2!=endpatt.ny%2))||
		(initsymm==1&&(arr[0].nx%2!=endpatt.nx%2||arr[0].ny%2!=endpatt.ny%2))
		)
		{
			cout<<"Impossible to get from start to target in INT."<<endl;
			exit(0);
		}
	}
	cout << "initsymm: " << initsymm << endl << endl;
	outfile.open(file,ios_base::app);
	if(s5flag) outfile<<"#";
	outfile << RLE << endl << endl;
	//cout << nflag << endl;
	b1efrontend=checkfrontend(arr[0],1);
	b1e2cfrontend=checkfrontend(arr[0],3);
	b2afrontend=checkfrontend(arr[0],4);
	b2a2cfrontend=checkfrontend(arr[0],6);
	b2a3ifrontend=checkfrontend(arr[0],12);
	
	//cout << b1efrontend << "," << b1e2cfrontend << "," << b2afrontend << "," << b2a2cfrontend << "," << b2a3ifrontend << endl;
	
	if(!endpatt.nx)
	{
		if(b1efrontend&(b1efrontend-1)) b1efrontend=0;
		if(b1e2cfrontend&(b1e2cfrontend-1)) b1e2cfrontend=0;
		if(b2afrontend&(b2afrontend-1)) b2afrontend=0;
		if(b2a2cfrontend&(b2a2cfrontend-1)) b2a2cfrontend=0;
		if(b2a3ifrontend&(b2a3ifrontend-1)) b2a3ifrontend=0;
	}
	
	int n=0;
	if ((arr[0].translist[2]!=1 || arr[0].translist[6]!=1) || endpatt.nx!=0 )
	{
		#ifndef NORECURSE
		branch(0);
		#endif
		#ifdef NORECURSE
		int s=-1;
		if(n==0&&loadloc!="")
		{
			s = load();
		}
		array<int,103>* trstr = new array<int,103>[MAXGEN];
		while(n>=0)
		{
			outside:
			trstr[n] = arr[n].totrans();
			postotalindex[n][1] = intpow(trstr[n][102]);
			if(n<=s)
			{
				if(n==s)
					s=-1;
			}
			else
			{
				postotalindex[n][0]=0;
			}
			while(postotalindex[n][0]<postotalindex[n][1])
			{
				inside:
				if(postotalindex[n][0]>=postotalindex[n][1]) break;
				if (nflag>0 && pattcount>=nflag) break;
				if(n>s)
					inc++;
				if(inc%5000000==0&&inc)
				{
					if(backuploc!="")
						save(n);
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
				combo(arr[n],trstr[n],postotalindex[n][0]);
				bool o = checktrans(q,n);
				if(o && n+1<MAXGEN)
				{
					arr[n+1]=q;
					n+=1;
					goto outside;
				}
				if (postotalindex[n][0]<postotalindex[n][1])
				{
					postotalindex[n][0]+=1;
				}
			}
			n--;
			if(n<0)
			{
				break;
			}
			if (postotalindex[n][0]<postotalindex[n][1])
			{
				postotalindex[n][0]+=1;
				goto inside;
			}
		}	
		delete[] trstr;
		#endif
	}
	#ifdef NORECURSE
	delete[] arr;
	#endif
	outfile.close();
	if(backuploc!="")
	{
		backupfile.close();
	}
	cout << endl << endl;
	cout << "time: " << chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - t1).count() << " seconds, patterns: " << pattcount << ", inc: " << inc << endl;
}
