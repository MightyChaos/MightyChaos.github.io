%
%Author: WangChaoyang 2013-02-17
%Input:
%   bundle: a matrix cell of grayscale images
%   mask:   a uint8 mask of region of interest
%   Z:  a matrix of depth map
%   cx,cy,f: scalars of camera parameters
%Outpur:
%   N:  h*w*3 double matrix of normal map
%   A:  3*3 matrix
%   L:  3*fnum matrix
%   X_smooth, Y_smooth, Z_smooth form 3-d coordinates of smoothed
%   pointcloud
%   IN: rgb image visialize normal map N
%   INd: rgb image visialize normal map Nd
%   Nd: h*w*3 matrix of normal map estimate from depth map
%
function [N A L X_smooth Y_smooth Z_smooth IN INd Nd] = PhotometricStereo(bundle,mask,Z,cx,cy,f)
    [h w] = size(bundle{1});
    pnum = h * w;
    fnum = size(bundle,1);
    II1 = zeros(pnum,fnum);
    for i=1:fnum
        II1(:,i) = double(reshape(bundle{i},pnum,1));
    end
    [s L] = cyDecompPhotometric(II1);
    S = zeros(h,w,3);
    S(:,:,1) = reshape(s(:,1),h,w);
    S(:,:,2) = reshape(s(:,2),h,w);
    S(:,:,3) = reshape(s(:,3),h,w);

    %smooth depth map
    Z_smooth = cyDepthDenoise(Z,S);
    U = repmat(1:w,[h 1]) - 0.5;
    V = repmat((1:h)',[1 w]) - 0.5;
    X_smooth = Z.*(U-cx)/f;
    Y_smooth = Z.*(V-cy)/f;
    
    %calibrate normal
    [N Nd A L] = cyUnambNormal(s, L, X_smooth, Y_smooth, Z_smooth, mask>0);
    IN = cyNormalToRGB(N);
    INd = cyNormalToRGB(Nd);

end