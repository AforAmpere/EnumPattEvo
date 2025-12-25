#include "src/arrayfuncs.h"

#ifndef RANGE
#define RANGE 1
#endif

#define H_X(n) ((n dims_x+1)/2+RANGE)
#define H_Y(n) ((n dims_y+1)/2+RANGE)

#define E_X(n) (n dims_x+2*RANGE)
#define E_Y(n) (n dims_y+2*RANGE)

#define P_LX(n) RANGE
#define P_LY(n) RANGE
#define P_RX(n) (n dims_x+RANGE)
#define P_RY(n) (n dims_y+RANGE)

void CellArray::generate_trans_array(RulespaceRep& rulespace, TransSetArray& visited)
{
	int tmp=0;
	//Rulespace specific
	static grid tmpgrid;
	tmpgrid.resize(501+4*RANGE, vector<int>(501+4*RANGE,0));

	for(int i=RANGE+1;i<E_X()+2*RANGE+1;i++)
	{
		for(int j=RANGE+1;j<E_Y()+2*RANGE+1;j++)
		{
			tmpgrid[i][j] = cells[i-RANGE-1][j-RANGE-1]+tmpgrid[i-1][j]+tmpgrid[i][j-1]-tmpgrid[i-1][j-1];
		}
	}

	for(int i=E_X()-1;i>=0;i--)
	{
		for(int j=E_Y()-1;j>=0;j--)
		{
			tmp=(TRANSCOUNT/2-1)*cells[i][j]+tmpgrid[i+2*RANGE+1][j+2*RANGE+1]+tmpgrid[i][j]-tmpgrid[i][j+2*RANGE+1]-tmpgrid[i+2*RANGE+1][j];
			trans[i][j]=tmp;
			//cout<<i<<","<<j<<","<<tmp<<endl;
			if (rulespace[tmp][0]>1&&visited[tmp]==0) 
			{
				visited[tmp]=1;
			}
		}
	}
}

bool compare_cell_arrays(CellArray& arr1, CellArray& arr2)
{
	if(arr1.pop!=arr2.pop) return 0;
	if(arr1.symm!=arr2.symm) return 0;
	if(arr1.dims_x!=arr2.dims_x) return 0;
	if(arr1.dims_y!=arr2.dims_y) return 0;
	int dims_x = arr1.dims_x;
	int dims_y = arr1.dims_y;
	switch(arr1.symm)
	{
		case 9:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<i+1;j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 8:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=i;j<E_Y()-i;j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 7:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<H_Y();j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 6:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<H_Y();j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 5:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<E_Y()-i;j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 4:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<i+1;j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 3:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<H_Y();j++)
				{
					if(arr1. cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 2:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<P_RY();j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 1:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<P_RY();j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
		case 0:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<P_RY();j++)
				{
					if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
				}
			}
			break;
		}
	}
	return 1;
}

bool compare_cell_arrays(CellArray& arr1, CellArray& arr2, CellArray& mask, int offx, int offy)
{
	int min_x=SMOLL;
	int min_y=SMOLL;
	int max_x=LORGE;
	int max_y=LORGE;

	bool ex=false;
	
	for(int i=P_LX(arr1.);i<P_RX(arr1.)&&!ex;i++)
	{
		for(int j=P_LY(arr1.);j<P_RY(arr1.)&&!ex;j++)
		{
			if(i+offx>=RANGE&&j+offy>=RANGE&&i+offx<RANGE+mask.dims_x&&j+offy<RANGE+mask.dims_y&&mask.cells[i+offx][j+offy]&&arr1.cells[i][j])
			{
				min_x=i;
				ex=true;
			}
		}
	}
	if(min_x==SMOLL)
	{
		return arr2.pop==0;
	}
	ex=false;
	for(int j=P_LY(arr1.);j<P_RY(arr1.)&&!ex;j++)
	{
		for(int i=min_x;i<P_RX(arr1.)&&!ex;i++)
		{
			if(i+offx>=RANGE&&j+offy>=RANGE&&i+offx<RANGE+mask.dims_x&&j+offy<RANGE+mask.dims_y&&mask.cells[i+offx][j+offy]&&arr1.cells[i][j])
			{
				min_y=j;
				ex=true;
			}
		}
	}
	ex=false;
	for(int j=P_RY(arr1.)-1;j>=min_y&&!ex;j--)
	{
		for(int i=min_x;i<P_RX(arr1.)&&!ex;i++)
		{
			if(i+offx>=RANGE&&j+offy>=RANGE&&i+offx<RANGE+mask.dims_x&&j+offy<RANGE+mask.dims_y&&mask.cells[i+offx][j+offy]&&arr1.cells[i][j])
			{
				max_y=j;
				ex=true;
			}
		}
	}
	ex=false;
	for(int i=P_RX(arr1.)-1;i>=min_x&&!ex;i--)
	{
		for(int j=min_y;j<max_y+1&&!ex;j++)
		{
			if(i+offx>=RANGE&&j+offy>=RANGE&&i+offx<RANGE+mask.dims_x&&j+offy<RANGE+mask.dims_y&&mask.cells[i+offx][j+offy]&&arr1.cells[i][j])
			{
				max_x=i;
				ex=true;
			}
		}
	}

	if(max_x-min_x+1!=arr2.dims_x||max_y-min_y+1!=arr2.dims_y) return 0;

	for(int i=min_x;i<max_x+1;i++)
	{
		for(int j=min_y;j<max_y+1;j++)
		{
			if((mask.cells[i+offx][j+offy]&&arr1.cells[i][j]!=arr2.cells[i-min_x+RANGE][j-min_y+RANGE])) return 0;
		}
	}
	return 1;
}

inline void find_bounds(RulespaceRep& rulespace, CellArray& prevarray, int& low_x, int& low_y, int& w_x, int& w_y)
{
	int dims_x=prevarray.dims_x;
	int dims_y=prevarray.dims_y;
	switch(prevarray.symm)
	{
		case 9:
		{
			for(int j=0;j<H_Y();j++)
			{
				for(int i=j;i<H_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=j;
						low_y=j;
						w_x=E_Y()-2*j;
						w_y=w_x;
						return;
					}
				}
			}
			break;
		}

		case 8:
		{
			for(int i=0;i<H_X();i++)
			{
				for(int j=i;j<E_Y()-i;j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=i;
						low_y=i;
						w_x=E_X()-2*i;
						w_y=w_x;
						return;
					}
				}
			}
			break;
		}

		case 7:
		{
			for(int i=0;i<H_X();i++)
			{
				for(int j=0;j<H_Y();j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=i;
						w_x=E_X()-2*i;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(w_x==0) return;
			for(int j=0;j<H_Y();j++)
			{
				for(int i=low_x;i<H_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_y=j;
						w_y=E_Y()-2*j;
						return;
					}
				}
			}
			break;
		}

		case 6:
		{
			for(int i=0;i<H_X();i++)
			{
				for(int j=0;j<H_Y();j++)
				{
					//Scan both top and left simultaneously
					if(rulespace[prevarray.trans[i][j]][0]||rulespace[prevarray.trans[j][i]][0])
					{
						low_x=i;
						low_y=i;
						w_x=E_X()-2*i;
						w_y=w_x;
						return;
					}
				}
			}
			break;
		}

		case 5:
		{
			for(int i=0;i<E_X();i++)
			{
				for(int j=0;j<E_Y()-i;j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=i;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(low_x==LORGE) return;
			for(int j=0;j<E_Y();j++)
			{
				for(int i=low_x;i<E_X()-j;i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_y=j;
						w_y=E_Y()-low_y-low_x;
						w_x=w_y;
						return;
					}
				}
			}
			break;
		}

		case 4:
		{
			for(int j=0;j<E_Y();j++)
			{
				for(int i=j;i<E_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=j;
						low_y=j;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(low_x==LORGE) return;
			for(int i=E_Y()-1;i>=low_x;i--)
			{
				for(int j=low_y;j<i+1;j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						w_x=i-low_x+1;
						w_y=w_x;
						return;
					}
				}
			}
			break;
		}

		case 3:
		{
			for(int i=0;i<E_X();i++)
			{
				for(int j=0;j<H_Y();j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=i;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(low_x==LORGE) return;
			for(int j=0;j<H_Y();j++)
			{
				for(int i=low_x;i<E_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_y=j;
						w_y=E_Y()-2*j;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			for(int i=E_X()-1;i>=low_x;i--)
			{
				for(int j=low_y;j<H_Y();j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						w_x=i-low_x+1;
						return;
					}
				}
			}
			break;
		}

		case 2:
		{
			for(int i=0;i<H_X();i++)
			{
				for(int j=0;j<E_Y();j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=i;
						w_x=E_X()-2*i;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(low_x==LORGE) return;
			for(int j=0;j<E_Y();j++)
			{
				for(int i=low_x;i<H_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_y=j;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			for(int j=E_Y()-1;j>=low_y;j--)
			{
				for(int i=low_x;i<H_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						w_y=j-low_y+1;
						return;
					}
				}
			}
			break;
		}

		case 1:
		{
			for(int i=0;i<H_X();i++)
			{
				for(int j=0;j<E_Y();j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=i;
						w_x=E_X()-2*i;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(low_x==LORGE) return;
			for(int j=0;j<E_Y();j++)
			{
				for(int i=low_x;i<H_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_y=j;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(low_y==0) 
			{
				w_y=E_Y();
				return;
			}
			for(int j=E_Y()-1;j>=E_Y()-low_y;j--)
			{
				for(int i=low_x;i<H_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_y=E_Y()-j-1;
						w_y=E_Y()-2*low_y;
						return;
					}
				}
			}
			w_y=E_Y()-2*low_y;
			return;
			break;
		}

		case 0:
		{
			for(int i=0;i<E_X();i++)
			{
				for(int j=0;j<E_Y();j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_x=i;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			if(low_x==LORGE) return;
			for(int j=0;j<E_Y();j++)
			{
				for(int i=low_x;i<E_X();i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						low_y=j;
						i=LORGE;
						j=LORGE;
					}
				}
			}
			for(int i=E_X()-1;i>=low_x;i--)
			{
				for(int j=low_y;j<E_Y();j++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						w_x=i-low_x+1;
						i=SMOLL;
						j=LORGE;
					}
				}
			}
			for(int j=E_Y()-1;j>=low_y;j--)
			{
				for(int i=low_x;i<low_x+w_x;i++)
				{
					if(rulespace[prevarray.trans[i][j]][0])
					{
						w_y=j-low_y+1;
						return;
					}
				}
			}
			break;
		}
	}
}

void evolve(RulespaceRep& rulespace, CellArray& prevarray, CellArray& newarray)
{
	int dims_x=0,dims_y=0;
	int low_x=LORGE, low_y=LORGE;
	int pop=0;
	find_bounds(rulespace, prevarray, low_x, low_y, dims_x, dims_y);
	clear_grid<grid>(newarray.cells, newarray.dims_x, newarray.dims_y);
	if(dims_x==0)
	{
		newarray.pop=0;
		newarray.symm=9;
		return;
	}
	int state=0;
	switch(prevarray.symm)
	{
		case 9:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<i+1;j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if((i!=j)&&(i!=E_X()-i-1)) 
						{
							pop+=8;
							newarray.cells[i][j]=state;
							newarray.cells[j][i]=state;
							newarray.cells[E_X()-i-1][j]=state;
							newarray.cells[j][E_X()-i-1]=state;
							newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
							newarray.cells[E_Y()-j-1][E_X()-i-1]=state;
							newarray.cells[i][E_Y()-j-1]=state;
							newarray.cells[E_Y()-j-1][i]=state;
						}
						else if((i==j)&&(i!=E_X()-i-1)) 
						{
							pop+=4;
							newarray.cells[i][j]=state;
							newarray.cells[E_X()-i-1][j]=state;
							newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
							newarray.cells[i][E_Y()-j-1]=state;
						}
						else if((i!=j)&&(i==E_X()-i-1)) 
						{
							pop+=4;
							newarray.cells[i][j]=state;
							newarray.cells[j][i]=state;
							newarray.cells[i][E_Y()-j-1]=state;
							newarray.cells[E_Y()-j-1][i]=state;
						}
						//if((i==j)&&(i==E_X()-i-1)) 
						else 
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}
					}
				}
			}
			break;
		}
		case 8:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=i;j<E_Y()-i;j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if((i!=j)&&(i!=E_Y()-j-1))
						{
							pop+=4;
							newarray.cells[i][j]=state;
							newarray.cells[j][i]=state;
							newarray.cells[E_Y()-j-1][E_X()-i-1]=state;
							newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
						}
						else if((i==j)!=(i==E_Y()-j-1))
						{
							pop+=2;
							newarray.cells[i][j]=state;
							newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
						}
						else
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}

					}
				}
			}
			break;
		}
		case 7:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<H_Y();j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if((i!=E_X()-i-1)&&(j!=E_Y()-j-1))
						{
							pop+=4;
							newarray.cells[i][j]=state;
							newarray.cells[E_X()-i-1][j]=state;
							newarray.cells[i][E_Y()-j-1]=state;
							newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
						}
						else if((i==E_X()-i-1)!=(j==E_Y()-j-1))
						{
							pop+=2;
							newarray.cells[i][j]=state;
							newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
						}
						else
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}
					}
				}
			}
			break;
		}
		case 6:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<(dims_y/2)+RANGE;j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						pop+=4;
						newarray.cells[i][j]=state;
						newarray.cells[j][E_X()-i-1]=state;
						newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
						newarray.cells[E_Y()-j-1][i]=state;
					}
				}
			}
			state=-rulespace[prevarray.trans[H_X()-1][H_Y()-1]][0];
			if((dims_x%2)&&state)
			{
				pop+=1;
				newarray.cells[H_X()-1][H_Y()-1]=state;
			}
			break;
		}
		case 5:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<E_Y()-i;j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if(i!=E_Y()-j-1)
						{
							pop+=2;
							newarray.cells[i][j]=state;
							newarray.cells[E_Y()-j-1][E_X()-i-1]=state;
						}
						else
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}
					}
				}
			}
			break;
		}
		case 4:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<i+1;j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if(i!=j)
						{
							pop+=2;
							newarray.cells[i][j]=state;
							newarray.cells[j][i]=state;
						}
						else
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}
					}
				}
			}
			break;
		}
		case 3:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<H_Y();j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if(j!=E_Y()-j-1)
						{
							pop+=2;
							newarray.cells[i][j]=state;
							newarray.cells[i][E_Y()-j-1]=state;
						}
						else
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}
					}
				}
			}
			break;
		}
		case 2:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<P_RY();j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if(i!=E_X()-i-1)
						{
							pop+=2;
							newarray.cells[i][j]=state;
							newarray.cells[E_X()-i-1][j]=state;
						}
						else
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}
					}
				}
			}
			break;
		}
		case 1:
		{
			for(int i=P_LX();i<H_X();i++)
			{
				for(int j=P_LY();j<P_RY();j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						if(i!=E_X()-i-1)
						{
							pop+=2;
							newarray.cells[i][j]=state;
							newarray.cells[E_X()-i-1][E_Y()-j-1]=state;
						}
						else
						{
							pop+=1;
							newarray.cells[i][j]=state;
						}
					}
				}
			}
			break;
		}
		case 0:
		{
			for(int i=P_LX();i<P_RX();i++)
			{
				for(int j=P_LY();j<P_RY();j++)
				{
					state=-rulespace[prevarray.trans[low_x+i-P_LX()][low_y+j-P_LY()]][0];
					if(state)
					{
						pop+=1;
						newarray.cells[i][j]=state;
					}
				}
			}
			break;
		}
	}
	newarray.dims_x=dims_x;
	newarray.dims_y=dims_y;
	newarray.pop=pop;
	newarray.disp_x=prevarray.disp_x+low_x-RANGE;
	newarray.disp_y=prevarray.disp_y+low_y-RANGE;
	newarray.symm=prevarray.symm;
}

int determine_symm(CellArray& arr)
{
	bool rotcw=1,xflip=1,yflip=1,xyflip=1,ldaxisflip=1,rdaxisflip=1;
	
	if(arr.dims_x!=arr.dims_y)
	{
		rotcw=0;
		ldaxisflip=0;
		rdaxisflip=0;
		for(int j=P_LY(arr.);j<P_RY(arr.);j++)
		{
			for(int i=P_LX(arr.);i<P_RX(arr.);i++)
			{
				if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][j]) xflip=0;
				if (arr.cells[i][j]!=arr.cells[i][E_Y(arr.)-j-1]) yflip=0;
				if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][E_Y(arr.)-j-1]) xyflip=0;
			}
		}
	}
	else
	{
		for(int j=P_LY(arr.);j<P_RY(arr.);j++)
		{
			for(int i=P_LX(arr.);i<P_RX(arr.);i++)
			{
				if (arr.cells[i][j]!=arr.cells[E_Y(arr.)-j-1][i]) rotcw=0;
				if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][j]) xflip=0;
				if (arr.cells[i][j]!=arr.cells[i][E_Y(arr.)-j-1]) yflip=0;
				if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][E_Y(arr.)-j-1]) xyflip=0;
				if (arr.cells[i][j]!=arr.cells[j][i]) ldaxisflip=0;
				if (arr.cells[i][j]!=arr.cells[E_Y(arr.)-j-1][E_X(arr.)-i-1]) rdaxisflip=0;
			}
		}
	}
	
	if(rotcw)
	{
		if(xflip)
		{
			return 9; //D8
		}
		return 6; //C4
	}
	if(xflip)
	{
		if(yflip)
		{
			return 7; //D4+
		}
		return 2; //D2|
	}
	if(yflip) return 3; //D2-
	if(ldaxisflip)
	{
		if(rdaxisflip)
		{
			return 8; //D4x
		}
		return 4; //D2\  /
	}
	if(rdaxisflip) return 5; //D2/
	if(xyflip) return 1; //C2
	return 0; //C1
}

bool symmetry_check(CellArray& arr)
{
	bool rotcw=1,xflip=1,yflip=1,xyflip=1,ldaxisflip=1,rdaxisflip=1;
	switch(arr.symm){
		case 9: //D8
		{
			return 1;
		}
		case 8: //D4x
		{
			for(int i=P_LX(arr.);i<H_X(arr.);i++)
			{
				for(int j=i;j<E_Y(arr.)-i;j++)
				{
					if(arr.cells[i][j]!=arr.cells[E_Y(arr.)-j-1][i]) return 1; //rotcw
				}
			}
			return 0;
		}
		case 7: //D4+
		{
			if (arr.dims_x!=arr.dims_y) return 1;
			for(int i=P_LX(arr.);i<H_X(arr.);i++)
			{
				for(int j=P_LY(arr.);j<H_Y(arr.);j++)
				{
					if(arr.cells[i][j]!=arr.cells[j][i]) return 1; //ldaxisflip
				}
			}
			return 0;
		}
		
		case 6: //C4
		{
			for(int i=P_LX(arr.);i<H_X(arr.);i++)
			{
				for(int j=P_LY(arr.);j<H_Y(arr.);j++)
				{
					if(arr.cells[i][j]!=arr.cells[j][i]) return 1; //ldaxisflip
				}
			}
			return 0;
		}
		case 5: //D2/
		{
			for(int i=P_LX(arr.);i<P_RX(arr.);i++)
			{
				for(int j=P_LY(arr.);j<E_Y(arr.)-i;j++)
				{
					if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][E_Y(arr.)-j-1]) return 1; //xyflip
				}
			}
			return 0;
		}
		case 4: //D2\ /
		{
			for(int i=P_LX(arr.);i<P_RX(arr.);i++)
			{
				for(int j=P_LY(arr.);j<i+1;j++)
				{
					if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][E_Y(arr.)-j-1]) return 1; //xyflip
				}
			}
			return 0;
		}
		case 3: //D2-
		{
			for(int i=P_LX(arr.);i<P_RX(arr.);i++)
			{
				for(int j=P_LY(arr.);j<H_Y(arr.);j++)
				{
					if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][E_Y(arr.)-j-1]) return 1; //xyflip
				}
			}
			return 0;
		}
		case 2: //D2|
		{
			for(int i=P_LX(arr.);i<H_X(arr.);i++)
			{
				for(int j=P_LY(arr.);j<P_RY(arr.);j++)
				{
					if (arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][E_Y(arr.)-j-1]) return 1; //xyflip
				}
			}
			return 0;
		}
		case 1: //C2
		{
			for(int i=P_LX(arr.);i<P_RX(arr.);i++)
			{
				for(int j=P_LY(arr.);j<P_RY(arr.);j++)
				{
					if (rotcw&&arr.cells[i][j]!=arr.cells[E_Y(arr.)-j-1][i]) rotcw=0;
					if (xflip&&arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][j]) xflip=0;
					if (ldaxisflip&&(arr.dims_x!=arr.dims_y||arr.cells[i][j]!=arr.cells[j][i])) ldaxisflip=0;
				}
			}
			if(rotcw||xflip||ldaxisflip) return 0;
			return 1;
		}
		
		case 0: //C1
		{
			for(int i=P_LX(arr.);i<P_RX(arr.);i++)
			{
				for(int j=P_LY(arr.);j<P_RY(arr.);j++)
				{
					if (xyflip&&arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][E_Y(arr.)-j-1]) xyflip=0;
					if (xflip&&arr.cells[i][j]!=arr.cells[E_X(arr.)-i-1][j]) xflip=0;
					if (yflip&&arr.cells[i][j]!=arr.cells[i][E_Y(arr.)-j-1]) yflip=0;
					if (ldaxisflip&&(arr.dims_x!=arr.dims_y||arr.cells[i][j]!=arr.cells[j][i])) ldaxisflip=0;
					if (rdaxisflip&&(arr.dims_x!=arr.dims_y||arr.cells[i][j]!=arr.cells[E_Y(arr.)-j-1][E_X(arr.)-i-1])) rdaxisflip=0;
				}
			}
			if(xyflip||xflip||yflip||ldaxisflip||rdaxisflip) return 0;
			return 1;
		}
	}
	return 0;
}

bool possible_symm_transition(int curr, int targ)
{
	static array<array<int,10>,10> tab{{
	{{1,1,1,1,1,1,1,1,1,1}},
	{{0,1,0,0,0,0,1,1,1,1}},
	{{0,0,1,0,0,0,0,1,0,1}},
	{{0,0,0,1,0,0,0,1,0,1}},
	{{0,0,0,0,1,0,0,0,1,1}},
	{{0,0,0,0,0,1,0,0,1,1}},
	{{0,0,0,0,0,0,1,0,0,1}},
	{{0,0,0,0,0,0,0,1,0,1}},
	{{0,0,0,0,0,0,0,0,1,1}},
	{{0,0,0,0,0,0,0,0,0,1}}
	}};
	return tab[curr][targ];
}

string canonized_speed(int dx, int dy, int period)
{
	stringstream s;
	s<<max(abs(dx),abs(dy))<<", "<<min(abs(dx),abs(dy))<<", "<<period;
	return s.str();
}

void canonize_array(CellArray& inarr, CellArray& outarr, int disp_x, int disp_y)
{
	int adx = max(abs(disp_x),abs(disp_y));
	int ady = min(abs(disp_x),abs(disp_y));

	outarr.pop=inarr.pop;

	auto state=+[](CellArray& arr, CellArray& inarr, int i, int j){return (int*)nullptr;};
	if(adx==disp_x)
	{
		if(ady==disp_y)
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[i][j];};
			outarr.dims_x=inarr.dims_x;
			outarr.dims_y=inarr.dims_y;
		}
		else
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[i][E_Y(inarr.)-j-1];};
			outarr.dims_x=inarr.dims_x;
			outarr.dims_y=inarr.dims_y;
		}
	}
	else if(adx==-disp_x)
	{
		if(ady==disp_y)
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[E_X(inarr.)-i-1][j];};
			outarr.dims_x=inarr.dims_x;
			outarr.dims_y=inarr.dims_y;
		}
		else
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[E_X(inarr.)-i-1][E_Y(inarr.)-j-1];};
			outarr.dims_x=inarr.dims_x;
			outarr.dims_y=inarr.dims_y;
		}
	}
	else if(adx==disp_y)
	{
		if(ady==disp_x)
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[j][i];};
			outarr.dims_x=inarr.dims_y;
			outarr.dims_y=inarr.dims_x;
		}
		else
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[j][E_X(inarr.)-i-1];};
			outarr.dims_x=inarr.dims_y;
			outarr.dims_y=inarr.dims_x;
		}
	}
	else if(adx==-disp_y)
	{
		if(ady==disp_x)
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[E_Y(inarr.)-j-1][i];};
			outarr.dims_x=inarr.dims_y;
			outarr.dims_y=inarr.dims_x;
		}
		else
		{
			state=[](CellArray& arr, CellArray& inarr, int i, int j){return &arr.cells[E_Y(inarr.)-j-1][E_X(inarr.)-i-1];};
			outarr.dims_x=inarr.dims_y;
			outarr.dims_y=inarr.dims_x;
		}
	}
	for(int i=RANGE;i<inarr.dims_x+RANGE;i++)
	{
		for(int j=RANGE;j<inarr.dims_y+RANGE;j++)
		{
			*state(outarr,inarr,i,j)=inarr.cells[i][j];
		}
	}
}

bool check_mask(CellArray& arr, CellArray& mask, int offx, int offy)
{
	if(offx<0||offy<0) return 0;
	for(int i=RANGE;i<arr.dims_x+RANGE;i++)
	{
		for(int j=RANGE;j<arr.dims_y+RANGE;j++)
		{
            if(arr.cells[i][j])
            {
                if(i+offx>=RANGE&&j+offy>=RANGE&&i+offx<RANGE+mask.dims_x&&j+offy<RANGE+mask.dims_y)
                {
                    if(mask.cells[i+offx][j+offy]==0) return 0;
                }
                else
                {
                    return 0;
                }
            }
		}
	}
	return 1;
}