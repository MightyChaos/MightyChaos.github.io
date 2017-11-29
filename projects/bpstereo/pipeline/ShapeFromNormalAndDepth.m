%
%Author: WangChaoyang 2013-02-17
%Input:
%   N:  h*w*3 matrix of normal map
%   X,Y,Z: matrices form 3-d coordinates of point cloud estimated from
%   binocular stereo vision
%   mask:  an uint8 mask of region of interest
%   lamda:  a scalar
%   itr:    number of iteration
%Output:
%   Px Py Pz: matrices forms 3-d coordinates of point cloud
%   outmask: an unint8 matrix of output mask
function [Px Py Pz outmask] = ShapeFromNormalAndDepth(N,X,Y,Z,mask,lamda, itr)
    outmask = mask;
    Px = X; Py = Y; Pz = Z;
    for i=1:itr
        [Px Py Pz outmask] = cyShapeRecstr2(N,Px,Py,Pz,outmask,lamda);
    end
end