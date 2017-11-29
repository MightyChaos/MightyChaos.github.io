%
%Author: WangChaoyang 2013-02-17
%Input:
%   I1, I2: a pair of grayscale images
%   mask: an uint8 mask of the region of interest
%   min_disp: a scalar, minimun disparity
%   max_disp: a scalar, maximun disparity
%   r:  a scalar, the radius of matching blocks
%   K:  a scalar, the number of candidate labels for each pixel
%   lamda:  a scalar
%   max_itr: a scalar, the number of max iteration of T-RWS
%   cx1, cx2, cy, f, b: scalars, camera parameters
%Output:
%   outmask: an uint8 mask of output region
%   D:  a double matrix, disparity map
%   X,Y,Z: double matrices, (X,Y,Z) forms 3d-coordinate.
%
function [outmask, D, X, Y, Z]=BinocularStereo(...
    I1, I2, mask, ...
    min_disp, max_disp,...
    r, K, lamda, max_itr, ...
    cx1, cx2, cy, f, b)

    [h w] = size(I1);
    minD = zeros(h,w) + min_disp;
    cost_vol = cyBuildCostVol_NCC(double(I1),...
        double(I2), minD, max_disp - min_disp, r);
    [valley_idx, data_term] = cyValley(cost_vol,K);
    valley_idx = valley_idx + min_disp -1;
    [label outmask] = cyMrfTrwOpt2(abs(valley_idx+cx2-cx1), data_term*lamda, r, h, w, K, max_itr, mask);
    outmask = cyDepthSeg(double(outmask)*2,outmask);
    label = label +1;
    D = zeros(h, w);
    for i=1:K
        idx = label == i;
        disp = valley_idx(i,:);
        D(idx) = disp(idx);
    end

    [X Y Z] = cyReprojectTo3d(1, D, cx1, cx2, cy, f, b);

end