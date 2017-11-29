#include <mex.h>
#include "cy_mexbasic.h"
#include "cy_basic.h"

void FillMask(bool**mask, int h, int w)
{
	int *min_i = new int[w];
	int *max_i = new int[w];
	int *min_j = new int[h];
	int *max_j = new int[h];
	//initialize 
	for(int i=0; i<w; i++)
	{
		min_i[i] = h;
		max_i[i] = 0;
	}
	for(int j=0; j<h; j++)
	{
		min_j[j] = w;
		max_j[j] = 0;
	}
	//compute min_i, min_j, max_i, max_j
	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++)
		{
			if(mask[i][j])
			{
				if(i<min_i[j])
					min_i[j] = i;
				if(i>max_i[j])
					max_i[j] = i;
				if(j<min_j[i])
					min_j[i] = j;
				if(j>max_j[i])
					max_j[i] = j;
			}
		}
	
	//fill holes
	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++)
		{
			if(!mask[i][j])
				if(i>=min_i[j] && i<=max_i[j] && j>=min_j[i] && j<=max_j[i])
					mask[i][j] = true;
		}
	delete[] min_i;
	delete[] max_i;
	delete[] min_j;
	delete[] max_j;
}

void mexFunction(int nlh, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs!=3)
		return;
	bool **mask = cyImportMask(prhs[0]);
	int h = (int)(*mxGetPr(prhs[1]));
	int w = (int)(*mxGetPr(prhs[2]));
	
	FillMask(mask, h, w);
	
	plhs[0] = cyExportBoolMat(mask,h,w);
	
	cyDeleteMat<bool>(mask);
}