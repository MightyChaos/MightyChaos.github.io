function [N Nd A L] = cyUnambNormal(S, L, X, Y, Z, idx)
    [h w] = size(X);
    dX = conv2(X,[0 1 -1],'same');
    dY = conv2(Y,[0; 1; -1], 'same');
    dZx = conv2(Z,[0 1 -1],'same');
    dZy = conv2(Z,[0; 1; -1],'same');
    nx_nz = -dZx./dX;
    ny_nz = -dZy./dY;
    Nz = sqrt(ones(h,w)./(nx_nz.*nx_nz+ny_nz.*ny_nz+1));
    idx2 = Nz>0;
    Nz(idx2) = -Nz(idx2);
    Nx = nx_nz.*Nz;
    Ny = ny_nz.*Nz;
    
    Sx = reshape(S(:,1),h,w);
    Sy = reshape(S(:,2),h,w);
    Sz = reshape(S(:,3),h,w);
    A = [Sx(idx) Sy(idx) Sz(idx)]\[Nx(idx) Ny(idx) Nz(idx)];
    
    L = inv(A)*L;
    S = S*A;
    alpha = sqrt(sum(S.*S,2));
    alpha = reshape(alpha,h,w);
    N = zeros(h,w,3);
    N(:,:,1) = reshape(S(:,1),h,w)./alpha;
    N(:,:,2) = reshape(S(:,2),h,w)./alpha;
    N(:,:,3) = reshape(S(:,3),h,w)./alpha;
    
    Nd = zeros(h,w,3);
    Nd(:,:,1) = Nx;
    Nd(:,:,2) = Ny;
    Nd(:,:,3) = Nz;
    
end