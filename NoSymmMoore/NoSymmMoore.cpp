#include "src/arrayfuncs.h"

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
    for(int i=0;i<E_X();i++)
    {
        for(int j=0;j<E_Y();j++)
        {
            tmp=get_cell_trans(i,j);
            trans[i][j]=tmp;
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

    for(int i=P_LX();i<P_RX();i++)
    {
        for(int j=P_LY();j<P_RY();j++)
        {
            if(arr1.cells[i][j]!=arr2.cells[i][j]) return 0;
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
		newarray.symm=0;
		return;
	}
	int state=0;

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

	newarray.dims_x=dims_x;
	newarray.dims_y=dims_y;
	newarray.pop=pop;
	newarray.disp_x=prevarray.disp_x+low_x-RANGE;
	newarray.disp_y=prevarray.disp_y+low_y-RANGE;
	newarray.symm=prevarray.symm;
}

int determine_symm(CellArray& arr)
{
	return 0; //C1
}

bool symmetry_check(CellArray& arr)
{
	return 1;
}

bool possible_symm_transition(int curr, int targ)
{
	return 1;
}

string canonized_speed(int dx, int dy, int period)
{
	stringstream s;
	s<<dx<<", "<<dy<<", "<<period;
	return s.str();
}

void canonize_array(CellArray& inarr, CellArray& outarr, int disp_x, int disp_y)
{
    outarr.dims_x=inarr.dims_x;
    outarr.dims_y=inarr.dims_y;
    outarr.pop=inarr.pop;
	for(int i=RANGE;i<inarr.dims_x+RANGE;i++)
	{
		for(int j=RANGE;j<inarr.dims_y+RANGE;j++)
		{
			outarr.cells[i][j]=inarr.cells[i][j];
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