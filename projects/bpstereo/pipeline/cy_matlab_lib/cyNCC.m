function ncc = cyNCC(sigma1,mean1,sigma2,mean2,I1,I2,D,r)
    [h w] = size(D);
    ncc = -ones(h, w);
    
    pu = repmat(1:w,[h 1]);
    pv = repmat((1:h)',[1 w]);
    qu = ceil(pu - D - 0.5);
    qv = pv;
    % discard pixels along the edges
    idx = pv>r & pv<h-r & pu>r & pu<w-r & qu>r & qu<w-r ;
    pu = pu(idx); pv = pv(idx);
    qu = qu(idx); qv = qv(idx);
    q = qv + (qu-1)*h;
    m1 = mean1(idx); 
    m2 = mean2(q); 
    t = zeros(size(m1));
    for i = -r:r
        for j = -r:r
            dp = pv+i+(pu-1+j)*h;
            dq = qv+i+(qu-1+j)*h;
            t = t + (I1(dp) - m1).*(I2(dq) - m2);
        end
    end
    
    s1 = sigma1(idx);
    s2 = sigma2(q);
    t = t./(s1.*s2);
    
    % if both pathces has zero var value,
    % then if their mean value deffirs in less than 10, they are regarded
    % as the same.
    is_zero_var1 = s1==0;
    is_zero_var2 = s2==0;
    is_zero_var = is_zero_var1 & is_zero_var2;
    threshold = 10;
    is_mean_close = abs(m1 - m2)<threshold;
    t(is_zero_var1) = -1;
    t(is_zero_var2) = -1;
    t(is_mean_close & is_zero_var) = 1;
    ncc(idx) = t;

end 