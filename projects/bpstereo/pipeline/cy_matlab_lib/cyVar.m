function var = cyVar(I,mean,r)
    [h w] = size(I);
    var = zeros(h,w);
    u = repmat(1:w,[h 1]);
    v = repmat((1:h)',[1 w]);
    idx = u>r & u<w-r & v>r & v<h-r;
    
    m = mean(idx);
    u = u(idx); v = v(idx);
    t = zeros(size(m));
    for i=-r:r
        for j=-r:r
            d = I(v+i+(u-1+j)*h) - m;
            t = t + d.*d;
        end
    end
    
    var(idx) = t;
end