function outmask =  cyDepthSeg(D,mask)
    [h w] = size(D);
    bsearched = false(h,w) | ~mask;
    min_m = 0;
    for i=1:h
       for j=1:w
%           fprintf('%d %d\n',i,j)
           if ~bsearched(i,j)
%               fprintf('%d %d\n',i,j)
               [mask coors] = cyDepthSegment(i,j,D,h,w);
               bsearched = bsearched | mask>0;
               m = size(coors,1);
               if m>min_m
                   min_m = m;
                   outmask = mask;
               end
           end
       end
    end
end