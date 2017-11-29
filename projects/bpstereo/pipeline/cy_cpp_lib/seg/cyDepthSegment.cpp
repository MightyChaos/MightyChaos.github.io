#include <mex.h>
#include "cy_mexbasic.h"
#include <cmath>
#include "DepthSegTree.h"
#include "cy_basic.h"

void mexFunction(int nlh, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs!=5)
		return;
	int h,w,i,j;
	double **depthmap;
	
	i = (int)(*mxGetPr(prhs[0]))-1;
	j = (int)(*mxGetPr(prhs[1]))-1;
	depthmap = cyImportDoubleMat(prhs[2]);
	h = (int)(*mxGetPr(prhs[3]));
	w = (int)(*mxGetPr(prhs[4]));
	
	DepthSegTree tree(j,i,depthmap,h,w);
//	mexPrintf("build: i:%d j%d, h%d, w%d\n", i, j, h, w);
	tree.Build();
//	mexPrintf("build end\n");
	bool** mask = tree.GetMask();
	int** coordinates;
	int size = tree.GetCoordinates(coordinates);
	plhs[0] = cyExportFloatMat(mask,h,w);
	plhs[1] = cyExportIntMat(coordinates,size,2);

	cyDeleteMat<double>(depthmap);
	cyDeleteMat<int>(coordinates);
}