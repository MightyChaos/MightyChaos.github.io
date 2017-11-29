function [Px Py Pz MskOut b] = cyShapeRecstr2(N,Zx,Zy,Z,Msk,lamda)
    [h w] = size(Zx);
    Pz = Z;
    U = Zx./Z;
    V = Zy./Z;
    DELTA = 0; % the default step legnth

    % Compute p and q (p=nx/nz, q=ny/nz)
    p=N(:,:,1)./N(:,:,3);
    q=N(:,:,2)./N(:,:,3);

    % Avoid invalid values
    nanidx= isnan(p); p(nanidx)=0;
    nanidx=isnan(q); q(nanidx)=0;
    infidx= isinf(p); p(infidx)=0;
    infidx=isinf(q); q(infidx)=0;

    [A_b bedge ]= cyLaplacian2(Msk);   
    
    dZx_r = conv2(Zx,[1, -1, 0],'same');
    idx = dZx_r < 0; dZx_r(idx) = DELTA;
    dZx_l = conv2(Zx,[0, 1, -1],'same');
    idx = dZx_l < 0; dZx_l(idx) = DELTA;
    dZy_b = conv2(Zy,[1; -1; 0],'same');
    idx = dZy_b < 0; dZy_b(idx) = DELTA;
    dZy_t = conv2(Zy,[0; 1; -1],'same');
    idx = dZy_t < 0; dZy_t(idx) = DELTA;

    dp = [zeros(h,1) p(:,1:w-1)].*dZx_l + conv2(p,[-1,0],'same').*dZx_r;
    dq = [zeros(1,w);q(1:h-1,:)].*dZy_t + conv2(q,[-1;0],'same').*dZy_b;
%     dp(bedge) = 0;
%     dq(bedge) = 0;
    b = dp + dq;
    b(b>5) = 0;
    b(b<-5) = 0;
%    b_b = dp(Msk>0)+dq(Msk>0);
    b_b = b(Msk>0);
    pnum = sum(sum(double(Msk>0)));
    A_t = speye(pnum,pnum)*lamda;
    b_t = Z(Msk>0)*lamda;

    
    disp('solving lstq...');
    pz = [A_t; A_b]\[b_t; b_b];

    Pz(Msk>0) = pz;
    Px = U.*Pz;
    Py = V.*Pz;
    
    MskOut = Msk; 
    MskOut(bedge) = 0;
end