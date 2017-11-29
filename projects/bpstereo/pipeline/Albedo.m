%
%Author: WangChaoyang
%Input:
%   bundle: bundle of rgb images
%   N: h*w*3 normal map
%   L: 3*fnum matrix
%Output:
%   albedo: h*w*3 uint8 matrix, mean of albedo_bundle
%   albedo_bundle: matrix cell of albedo evaluated from each images in bundle 
function [albedo albedo_bundle]=Albedo(bundle,N,L)
    [h w d] = size(bundle{1});
    fnum = size(bundle,1);
    albedo_bundle = cell(fnum,1);
    albedo = zeros(h, w, 3);
    for i=1:fnum
        albedo_bundle{i} = cyAlbedo(N,L(:,i),bundle{i});
        albedo = albedo + double(albedo_bundle{i});
    end
    albedo = uint8(albedo/fnum);
end