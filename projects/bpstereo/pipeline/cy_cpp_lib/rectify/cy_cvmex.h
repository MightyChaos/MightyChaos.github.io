#ifndef CY_CVMEX
#define CY_CVMEX
#endif
#include "cv.h"
#include "matrix.h"
#include "mex.h"
using namespace cv;

Mat cyLoadImg(const mxArray *arr);

mxArray* cyGetImg(Mat &mat);

Mat cyMxMatrixToMat(const mxArray *arr);
