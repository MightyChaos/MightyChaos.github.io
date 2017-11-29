function Vol = cyBuildCostVol_NCC(I1,I2, minD, vol_depth, r)
    [h w] = size(I1);
    N = boxfilter(ones(h,w),r);
    mean1 = boxfilter(I1,r)./N;
    mean2 = boxfilter(I2,r)./N;
    var1 = cyVar(I1,mean1,r);
    var2 = cyVar(I2,mean2,r);
    sigma1 = sqrt(var1);
    sigma2 = sqrt(var2);
    
    Vol = ones(vol_depth, h*w);
    for i=1:vol_depth
        ncc = cyNCC(sigma1, mean1,  sigma2, mean2, I1, I2, minD+i-1,r);
        cost = (1-ncc)/2;
        Vol(i,:) = cost(:);
    end
end