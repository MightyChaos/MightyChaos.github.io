% S should be a h*w*3 matrix, from cyDecompPhotometric
function Z = cyDepthDenoise(Z,S)
    [h w d] = size(Z);
    % scale S, so that s_i,j is between 0 and 1
    min_sx = min(min(S(:,:,1)));
    max_sx = max(max(S(:,:,1)));
    S(:,:,1) = (S(:,:,1)-min_sx)/(max_sx - min_sx);
    min_sy = min(min(S(:,:,2)));
    max_sy = max(max(S(:,:,2)));
    S(:,:,2) = (S(:,:,2)-min_sy)/(max_sy - min_sy);
    min_sz = min(min(S(:,:,3)));
    max_sz = max(max(S(:,:,3)));
    S(:,:,3) = (S(:,:,3)-min_sz)/(max_sz - min_sz);
    r = 6;
    eps = 0.001;
    S = cyDeNULL(S);
    S(:,:,1) = guidedfilter(S(:,:,1),S(:,:,1),r,eps);
%    Z = guidedfilter_color(S, Z, r, eps);
    Z = guidedfilter(S(:,:,1),Z,r,eps);
end