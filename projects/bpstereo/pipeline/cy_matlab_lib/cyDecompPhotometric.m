function [S L] = cyDecompPhotometric(I)
    [U Sigma V] = svd(I,'econ');
    U_p = U(:,1:3);
    Sigma_p=Sigma(1:3,1:3);
    V_p = V(:,1:3);  
    S = U_p*(sqrt(Sigma_p));
    L = sqrt(Sigma_p)*(V_p');   
end