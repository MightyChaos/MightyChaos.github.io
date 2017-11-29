function [L bedge E] = cyLaplacian(Msk)
    [h w] = size(Msk);
    bedge = Msk>0;
    pnum = sum(sum(double(Msk>0)));
    idx = zeros(h,w);
    idx(Msk>0) = 1:pnum;
    
    Lidx = zeros(5*pnum,2);
    Lval = zeros(5*pnum,1);
    Eidx = zeros(5*pnum,1);
    k = 0;
    ke = 0;
    
    for j=2:w-1
        for i=2:h-1
            if(Msk(i,j)>0 ...
                    && Msk(i, j-1)>0 ...
                    && Msk(i, j+1)>0 ...
                    && Msk(i+1, j)>0 ...
                    && Msk(i-1, j)>0)
                bedge(i,j) = false;
                ke = ke+1;
                Eidx(ke) = idx(i,j);
                %left
                k = k+1;
                Lidx(k,1) = idx(i,j);
                Lidx(k,2) = idx(i,j-1);
                Lval(k) = 1;               
                %right
                k=k+1;
                Lidx(k,1) = idx(i,j);
                Lidx(k,2) = idx(i,j+1);
                Lval(k) = 1;
                %up
                k=k+1;
                Lidx(k,1) = idx(i,j);
                Lidx(k,2) = idx(i-1,j);
                Lval(k) = 1;
                %down
                k=k+1;
                Lidx(k,1) = idx(i,j);
                Lidx(k,2) = idx(i+1,j);
                Lval(k) = 1;
                %center
                k=k+1;
                Lidx(k,1) = idx(i,j);
                Lidx(k,2) = idx(i,j);
                Lval(k) = -4;
                
            end
        end
    end
    
    L = sparse(Lidx(1:k,1),Lidx(1:k,2),Lval(1:k),pnum, pnum);
    E = sparse(Eidx(1:ke), Eidx(1:ke), 1, pnum, pnum);
end