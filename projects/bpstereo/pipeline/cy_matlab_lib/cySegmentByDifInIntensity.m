function Dif = cySegmentByDifInIntensity(bundle)
    fnum = size(bundle,1);
    [h w] = size(bundle{1});
    Dif = zeros(h,w);
    mean = zeros(h,w);
    for i=1:fnum
        mean = double(bundle{i}) + mean; 
    end
    mean = mean / fnum;
    for i=1:fnum
        dif = double(bundle{i})-mean;
        Dif = dif.*dif + Dif;
    end
    Dif = Dif/fnum;
   % Dif = abs(double(bundle{fnum})-mean);
end