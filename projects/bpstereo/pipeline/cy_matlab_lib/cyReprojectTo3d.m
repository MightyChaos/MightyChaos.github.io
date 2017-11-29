% author: Wang Chaoyang 2012-10-24


function [X Y Z] = cyReprojectTo3d(bleft, D, cx1, cx2, cy, f, b)
    D = double(D);
    [h w] = size(D);
    Z = (f*b)*ones(h,w)./(D+cx2-cx1);
    U = repmat(1:w,[h 1]) - 0.5;
    V = repmat((1:h)',[1 w]) - 0.5;
    if bleft >0
        cx = cx1;
    else
        cx = cx2;
    end
    X = ((U-cx)/f).*Z;
    Y = ((V-cy)/f).*Z;
end