%
% Author:?WangChaoyang 2013-02-17 
% input: 
%   bundle: a cell of gray scale images
%   theta:  a double scalar, threshold
% output:
%   mask: an uint8 matrix
%
function mask = Segmentation(bundle,theta)
    [h w] = size(bundle{1});   
    Dif = cySegmentByDifInIntensity(bundle);
    mask = cyDepthSeg(double(Dif>theta)*2, Dif>theta);
    mask = cyFillMaskHole(mask,h,w);
end