function R = cyDeNULL(A)
    idx = isnan(A) | isinf(A);
    A(idx) = 0;
    R = A;
end