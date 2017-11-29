function I = cyNormalToRGB(N)
    [h w c] = size(N);
    I = zeros(h,w,3, 'uint8');
    I(:,:,1) = uint8((N(:,:,2) + 1)/2 * 255);
    I(:,:,2) = uint8((N(:,:,1) + 1)/2 * 255);
    I(:,:,3) = uint8((-N(:,:,3) + 1)/2 * 255);  
end