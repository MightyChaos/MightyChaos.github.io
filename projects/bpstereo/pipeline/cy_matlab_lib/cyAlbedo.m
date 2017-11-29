function albedo = cyAlbedo(N,L,I)
   [h w d] = size(N);
   nx = N(:,:,1);
   ny = N(:,:,2);
   nz = N(:,:,3);
   
   L = L/sqrt(L'*L);
   
   s = nx(:)*L(1)+ny(:)*L(2)+nz(:)*L(3);
   s = reshape(s,h,w);
   r = double(I(:,:,1))./s;
   g = double(I(:,:,2))./s;
   b = double(I(:,:,3))./s;
   
   albedo = zeros(h,w,3);
   albedo(:,:,1) = r;
   albedo(:,:,2) = g;
   albedo(:,:,3) = b;
end