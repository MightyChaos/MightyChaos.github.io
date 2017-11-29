% vol is a d*pnum matrix, k is the number of valley we need for each pixel
% cost value in vol should be normalized between 0 to 1
% valley_idx is a pnum * k matrix of valley's coordinate
% data is a pnum * k matrix as the data term
function [valley_idx, data] = cyValley(vol,k)
    [d pnum] = size(vol);
    is_smaller_than_right = vol < [vol(2:d,:);zeros(1,pnum)];
    is_smaller_than_left  = vol < [zeros(1,pnum);vol(1:d-1,:)];
    is_valley = is_smaller_than_right & is_smaller_than_left;
    
    vol_only_valley = vol;
    %since cost value has been normalized, 
    %we set pixels not a valley to be the largest possible value, 1
    vol_only_valley(~is_valley) = 1;
    [data valley_idx] = sort(vol_only_valley);
    data = data(1:k,:);
    valley_idx = valley_idx(1:k,:);
    
    
end