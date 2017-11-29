#ifndef CY_MEXBASIC_H
#define CY_MEXBASIC_H
#endif
#include "matrix.h"
#include "mex.h"

mxArray* cyExportBoolMat(bool **mat, int h, int w);

mxArray* cyExportFloatMat(float **mat, int h, int w);

mxArray* cyExportIntMat(int **mat, int h, int w);


float** cyImportFloatMat(const mxArray *array);

double** cyImportDoubleMat(const mxArray *array);

int** cyImportIntMat(const mxArray *array);

bool** cyImportMask(const mxArray *arr);

unsigned char*** cyImportRgbImg(const mxArray *arr);