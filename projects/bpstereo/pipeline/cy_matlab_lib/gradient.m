function [G Gx Gy ] = gradient(m,n)
    Im = speye(m,m);
    Em = sparse(2:m,1:m-1,1,m,m);
    DDm = Em - Im;
    
    In = speye(n,n);
    En = sparse(2:n,1:n-1,1,n,n);
    DDn = En - In;
    
    Gx = kron(In,DDm);
    Gy = kron(DDn,Im);
    
    G = Gx+Gy;
end