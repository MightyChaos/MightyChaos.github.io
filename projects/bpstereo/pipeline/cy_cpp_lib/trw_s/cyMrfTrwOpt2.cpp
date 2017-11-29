#include "MRFEnergy.h"
#include "cy_mexbasic.h"
#include "cy_basic.h"
#include<matrix.h>
#include<mex.h>
#include<cmath>


inline double smooth_term(int a, int b, double va, double vb)
{
		double inv_va = 1/va;
		double inv_vb = 1/vb;
		return 2*abs( inv_va - inv_vb)/(inv_va + inv_vb);
}

inline void build_edges(MRFEnergy<TypeGeneral>* mrf, MRFEnergy<TypeGeneral>::NodeId node_a, 
	MRFEnergy<TypeGeneral>::NodeId node_b,
	int p, int q, int** valley_idx, int K)
{
	const int lnum = K;
	TypeGeneral::REAL *V = new TypeGeneral::REAL[lnum*lnum];
	for(int a=0; a<lnum; a++)
	{		
		for(int b=0; b<lnum; b++)
		{
			double va = 1; 
			double vb = 1;
			if(a<K)
				va = valley_idx[a][p];
			if(b<K)
				vb = valley_idx[b][q];
			V[a + b*lnum] = smooth_term(a, b, va, vb);
		}
	}
	mrf->AddEdge(node_a, node_b, TypeGeneral::EdgeData(TypeGeneral::GENERAL, V));
	delete[] V;
}

inline bool check_nghd(bool** mask, int u, int v, int r, int h, int w)
{
	for(int i=u-r; i<=u+r; i++)
		for(int j=v-r; j<=v+r; j++)
	{
		if(i>h-1 || i<0 || j>w-1 || j<0)
			return false;
		if(!mask[i][j])
			return false;
	}
	return true;
}
// this function is for 2d lattice mrf, espacially for stereo matching problem
// where neighboring vertices are just 4.
// valley_idx is a k*pnum int matrix, specify the k_th valley index
// data is a k*pnum double matrix, specify the uniry function in the mrf energy
// matrix if column-oriented. i.e. idx(i,j) = i + j*h
void trwOpt(int **&result, bool **&outmask, int** valley_idx, double** data, bool**mask,int r, int h, int w, int K, int max_itr)
{
	MRFEnergy<TypeGeneral>* mrf;
	MRFEnergy<TypeGeneral>::NodeId* nodes;
	MRFEnergy<TypeGeneral>::Options options;
	TypeGeneral::REAL energy, lowerBound;
	
	int pnum = h*w;
	mexPrintf("pnum: %d\n",pnum);
	nodes = new MRFEnergy<TypeGeneral>::NodeId[pnum];
	mrf = new MRFEnergy<TypeGeneral>(TypeGeneral::GlobalSize());
	//construct energy
	//for every vertex:
	//1. specify data term for each label
	//2. specify smooth term in the following form
	//   v(a,b) =  2* abs(1/f(a) - 1/f(b))/(1/f(a) + 1/f(b))  if a & b < K
	//	        =  phai                           if a | b == K
	//          =  0                              if a & b == K
	
	//1. specify data term
	outmask = cyCreateMat2<bool>(h,w,false);
	int node_num = 0;
	int** node_coors = cyCreateMat<int>(pnum,2);
	int* node_idx = new int[pnum];
	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++)
		{
			if(mask[i][j] && check_nghd(mask,i,j,r,h,w))
			{
				outmask[i][j] = true;
				TypeGeneral::REAL *D = new TypeGeneral::REAL[K];
				int p = i+j*h;
				for(int k=0; k<K; k++)
				{
					D[k] = data[k][p];
				}
				nodes[node_num] = mrf->AddNode(TypeGeneral::LocalSize(K), TypeGeneral::NodeData(D));
				node_idx[p] = node_num;
				node_coors[node_num][0] = i;
				node_coors[node_num][1] = j;
				node_num++;
			}
		}
	mexPrintf("add data term end\n");
	//2. specify smooth term
	for(int ni=0; ni<node_num; ni++)
	{
		int i = node_coors[ni][0];
		int j = node_coors[ni][1];
		int p = i+j*h;
		// down
		if(outmask[i+1][j])
		{
			build_edges(mrf, nodes[node_idx[p]], nodes[node_idx[p+1]], p, p+1, valley_idx, K);
		}
		// right
		if(outmask[i][j+1])
		{
			build_edges(mrf, nodes[node_idx[p]], nodes[node_idx[p+h]], p, p+h, valley_idx, K);
		}
	}
	mexPrintf("add smooth term end\n");
	options.m_iterMax = max_itr;
	mrf->Minimize_TRW_S(options, lowerBound, energy);
	
	//get solutions
	result = cyCreateMat2<int>(h,w,0);
	for(int ni=0; ni<node_num; ni++)
	{
		int i = node_coors[ni][0];
		int j = node_coors[ni][1];
		result[i][j] = mrf->GetSolution(nodes[ni]);
	}
	
	delete[] nodes;
	delete mrf;
	delete[] node_idx;
	cyDeleteMat<int>(node_coors);
}
//usage:
//result = cyMrfTrwOpt(valley_idx, data, r, h, w, K, max_itr, mask)
void mexFunction(int nlh, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs!=8)
		return;
	int **valley_idx;
	double **data;
	int h, r, w, K, max_itr;
	bool **mask;
	//retrieve data
	valley_idx = cyImportIntMat(prhs[0]);
	data = cyImportDoubleMat(prhs[1]);
	r = (int)(*mxGetPr(prhs[2]));
	h = (int)(*mxGetPr(prhs[3]));
	w = (int)(*mxGetPr(prhs[4]));
	K = (int)(*mxGetPr(prhs[5]));
	max_itr = (int)(*mxGetPr(prhs[6]));
	mask = cyImportMask(prhs[7]);
	
	int **solution; 
	bool **outmask;
	mexPrintf("h: %d w: %d\n",h,w);
	 
//	trwOpt(solution, valley_idx, data, phi, varphi,h, w, K, max_itr);
	trwOpt(solution, outmask, valley_idx, data, mask, r, h, w, K, max_itr);
	mexPrintf("trwOpt end\n");
	
	mexPrintf("start export result...\n");
	/*
	for(int i=0; i<h*w; i++)
	{
		mexPrintf("%d %d \n",i,solution[i]);
	}
	*/
	
	plhs[0] = cyExportIntMat(solution,h,w);
	plhs[1] = cyExportBoolMat(outmask,h,w);
	cyDeleteMat<int>(valley_idx);
	cyDeleteMat<double>(data);
	cyDeleteMat<bool>(mask);
	cyDeleteMat<bool>(outmask);
	cyDeleteMat<int>(solution);
	
}
