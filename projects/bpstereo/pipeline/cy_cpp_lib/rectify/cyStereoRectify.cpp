/**
 * cyStereoRectify
 *  input: CameraMat1, distCoeffs1, CameraMat2, distCoeffs2, R, T, limg, rimg
 *  output: Lrect, Rrect
 *  
 *  author: Wang Chaoyang 2012-10-23
 ***/

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <fstream>
#include <iostream>

#include<matrix.h>
#include<mex.h>
#include<math.h>
#include "cy_cvmex.h"

using namespace cv;
using namespace std;

struct camera_par
{
	Mat cameraMat1, cameraMat2, distCoeffs1, distCoeffs2, R , T;
};

void runStereoRectify(camera_par par, InputArray src1, InputArray src2, OutputArray out1, OutputArray out2, double &f, double &cx1, double &cx2, double &cy, double &b, bool bzerodisparity, bool bfullscreen)
{
	Mat R1, R2, P1, P2, Q;
	Rect roi1, roi2;
	Size size = src1.size();
 //   printf("%d %d",size.height, size.width);
//    printf("stereo rectify\n");
    int op = 0;
    int op2 = -1;
    if(bzerodisparity)
        op = CV_CALIB_ZERO_DISPARITY;
    if(bfullscreen)
        op2 = 0;
	stereoRectify(par.cameraMat1, par.distCoeffs1, par.cameraMat2, par.distCoeffs2, 
		size, 
		par.R, par.T, R1, R2, P1, P2, Q, 
		op , op2, size,
		&roi1, &roi2);
 //   printf("cx cy b\n");
    f = P1.at<double>(0,0);
    cx1 = P1.at<double>(0,2);
    cx2 = P2.at<double>(0,2);
    cy = P1.at<double>(1,2);
    b = 1.0/Q.at<double>(3,2);
	Mat rmap_l1, rmap_l2, rmap_r1, rmap_r2;
//    printf("initundistort\n");
	initUndistortRectifyMap(par.cameraMat1, par.distCoeffs1, R1, P1, size, CV_16SC2, rmap_l1, rmap_l2);
	initUndistortRectifyMap(par.cameraMat2, par.distCoeffs2, R2, P2, size, CV_16SC2, rmap_r1, rmap_r2);
	remap(src1, out1, rmap_l1, rmap_l2, CV_INTER_LINEAR);
	remap(src2, out2, rmap_r1, rmap_r2, CV_INTER_LINEAR);
 //   printf("run rectify end\n");
    cout<<"Q:\n"<<Q<<endl;
    mexPrintf("%f %f %f %f\n", Q.at<double>(0,0), Q.at<double>(0,1), Q.at<double>(0,2), Q.at<double>(0,3));
    mexPrintf("%f %f %f %f\n", Q.at<double>(1,0), Q.at<double>(1,1), Q.at<double>(1,2), Q.at<double>(1,3));
    mexPrintf("%f %f %f %f\n", Q.at<double>(2,0), Q.at<double>(2,1), Q.at<double>(2,2), Q.at<double>(2,3));
    mexPrintf("%f %f %f %f\n", Q.at<double>(3,0), Q.at<double>(3,1), Q.at<double>(3,2), Q.at<double>(3,3));
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    camera_par par;
//    printf("load par\n");
    par.cameraMat1 = cyMxMatrixToMat(prhs[0]);
    par.distCoeffs1 = cyMxMatrixToMat(prhs[1]);
    par.cameraMat2 = cyMxMatrixToMat(prhs[2]);
    par.distCoeffs2 = cyMxMatrixToMat(prhs[3]);
    par.R = cyMxMatrixToMat(prhs[4]);
    par.T = cyMxMatrixToMat(prhs[5]);
    double *oppr;
    bool bzerodisparity = true;
    bool bfullscreen = false;
    if(nrhs >= 9)
    {
        oppr = mxGetPr(prhs[8]);
        if(*oppr<=0)
            bzerodisparity = false;
    }
    if(nrhs == 10)
    {
        oppr = mxGetPr(prhs[9]);
        if(*oppr>0)
            bfullscreen = true;
    }
//    printf("load src\n");
    Mat src1 = cyLoadImg(prhs[6]);
    Mat src2 = cyLoadImg(prhs[7]);
    double f,cx1,cx2,cy,b;
    Mat L,R;
    runStereoRectify(par,src1,src2,L,R,f,cx1,cx2,cy,b,bzerodisparity,bfullscreen);
    plhs[0] = cyGetImg(L);
    plhs[1] = cyGetImg(R);
    plhs[2] = mxCreateDoubleScalar(f);
    plhs[3] = mxCreateDoubleScalar(cx1);
    plhs[4] = mxCreateDoubleScalar(cx2);
    plhs[5] = mxCreateDoubleScalar(cy);
    plhs[6] = mxCreateDoubleScalar(b);
}
