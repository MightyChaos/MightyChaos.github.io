#include "cy_mexbasic.h"
#include "cy_basic.h"

mxArray* cyExportBoolMat(bool **mat, int h, int w)
{
	mxArray* rt = mxCreateNumericMatrix(h,w,mxUINT8_CLASS,mxREAL);
	unsigned char *data = (unsigned char*)mxGetData(rt);
	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++)
		{
			if(mat[i][j])
				data[i+j*h] = 255;
			else
				data[i+j*h] = 0;
				
		}
	return rt;
}

mxArray* cyExportFloatMat(float **mat, int h, int w)
{
	mxArray* rt = mxCreateDoubleMatrix(h,w,mxREAL);
	double* data = (double*) mxGetData(rt);
	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++)
		{
			data[i+j*h] = mat[i][j];
		}
	return rt;
}

mxArray* cyExportIntMat(int **mat, int h, int w)
{
	mxArray* rt = mxCreateDoubleMatrix(h,w,mxREAL);
	double* data = (double*) mxGetData(rt);
	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++)
		{
			data[i+j*h] = (double)mat[i][j];
		}
	return rt;
}


float** cyImportFloatMat(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    float **rt = cyCreateMat<float>(h,w);
    double *data = (double*)mxGetData(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            rt[i][j] = data[i+j*h];
        }
    return rt;
}

bool** cyImportMask(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    bool **rt = cyCreateMat<bool>(h,w);
    unsigned char *data = (unsigned char*)mxGetData(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            if(data[i+j*h]>0)
                rt[i][j] = true;
            else
                rt[i][j] = false;
        }
    return rt;
}

unsigned char*** cyImportRgbImg(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    unsigned char ***rt = cyCreateVol<unsigned char>(h,w,3);
    unsigned char *data = (unsigned char*)mxGetData(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            rt[i][j][0] = data[i+j*h];
            rt[i][j][1] = data[i+j*h+h*w];
            rt[i][j][2] = data[i+j*h+2*h*w];
        }
    return rt;
}

double** cyImportDoubleMat(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    double **rt = cyCreateMat<double>(h,w);
    double *data = (double*)mxGetData(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            rt[i][j] = data[i+j*h];
        }
    return rt;
}

int** cyImportIntMat(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    int **rt = cyCreateMat<int>(h,w);
    double *data = (double*)mxGetData(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            rt[i][j] = (int)data[i+j*h];
        }
    return rt;
}
