#include "cy_cvmex.h"

Mat copyMexGrayImg(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    Mat rt(h,w,CV_8UC1);
    unsigned char *data = (unsigned char*)mxGetData(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            rt.at<unsigned char>(i,j) = data[i+j*h];
        }
    return rt;
}

Mat copyMexRgbImg(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    Mat rt(h,w,CV_8UC3);
    unsigned char *data = (unsigned char*)mxGetData(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            Vec<unsigned char,3> rgb(3);
            rgb(0) = data[i + j*h];
            rgb(1) = data[i + j*h + h*w];
            rgb(2) = data[i + j*h + 2*h*w];
            rt.at<Vec<unsigned char,3>>(i,j) = rgb;
        }
    return rt;
}

mxArray* copyCvGrayImg(Mat& mat)
{
    int h = mat.rows;
    int w = mat.cols;
    mxArray* rt = mxCreateNumericMatrix(h,w,mxUINT8_CLASS,mxREAL);
    unsigned char *data = (unsigned char*)mxGetData(rt);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            data[i+j*h] = mat.at<unsigned char>(i,j);
        }
    return rt;
}

mxArray* copyCvRgbImg(Mat& mat)
{
    int h = mat.rows;
    int w = mat.cols;
    mwSize *size = new mwSize[3];
    size[0] = h; size[1] = w; size[2] = 3;
    mxArray* rt = mxCreateNumericArray(3,size,mxUINT8_CLASS,mxREAL);
    unsigned char *data = (unsigned char*)mxGetData(rt);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            Vec<unsigned char,3> rgb = mat.at<Vec<unsigned char,3>>(i,j);
            data[i+j*h] = rgb[0];
            data[i+j*h+h*w] = rgb[1];
            data[i+j*h+2*h*w] = rgb[2];
        }
    return rt;
}


        
Mat cyLoadImg(const mxArray *arr)
{
    Mat rt;
    int dims = mxGetNumberOfDimensions(arr);
    if(dims == 1)
        rt = copyMexGrayImg(arr);
    if(dims == 3)
        rt = copyMexRgbImg(arr);
    return rt;
}

mxArray* cyGetImg(Mat &mat)
{
    int type = mat.type();
    if(type == CV_8UC1)
        return copyCvGrayImg(mat);
    if(type == CV_8UC3)
        return copyCvRgbImg(mat);
    return NULL;
}

Mat cyMxMatrixToMat(const mxArray *arr)
{
    const mwSize *size = mxGetDimensions(arr);
    int h = size[0];
    int w = size[1];
    Mat rt(h,w,CV_64F);
    double *data = (double*)mxGetPr(arr);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            rt.at<double>(i,j) = data[i+j*h];
        }
    return rt;
}