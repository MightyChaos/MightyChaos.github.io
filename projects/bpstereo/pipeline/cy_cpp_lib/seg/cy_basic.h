#ifndef CY_BASIC_H
#define CY_BASIC_H

template<typename T> T** cyCreateMat(int h, int w)
{
	T* data = new T[h*w];
	T** rt = new T*[h];
	for(int i=0; i<h; i++)
	{
		rt[i] = data+i*w;
	}
	return rt;
}

template<typename T> T** cyCreateMat2(int h, int w, T val)
{
	T** rt = cyCreateMat<T>(h,w);
	T* data = rt[0];
	for(int i=0; i<h*w; i++)
	{
		data[i]=val;
	}
	return rt;
}

template<typename T> void cyDeleteMat(T** mat)
{
	T* data = mat[0];
	delete[] data;
	delete[] mat; 
}

template<typename T> T*** cyCreateVol(int h, int w, int d)
{
	T* data = new T[h*w*d];
	T*** rt = cyCreateMat<T*>(h,w);
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<w; j++)
		{
			rt[i][j] = data+i*w*d+j*d;
		}
	}
	return rt;
}

template<typename T> T*** cyCreateVol2(int h, int w, int d, T val)
{
	T*** rt = cyCreateVol<T>(h,w,d);
	T*data = rt[0][0];
	for(int i=0; i<h*w*d; i++)
	{
		data[i]=val;
	}
	return rt;
}

template<typename T> void cyDeleteVol(T*** vol)
{
	T* data = vol[0][0];
	delete[] data;
	cyDeleteMat<T*>(vol);
}
#endif