Img = [0 0 0 0 0 0 0 1 1 0,
       1 0 0 1 0 0 1 0 0 1, 
       1 0 0 1 0 1 1 0 0 0,
       0 0 1 1 1 0 0 0 0 0,
       0 0 1 1 1 0 0 1 1 1];

fprintf('%d\n',nonzeroCount(Img(1:4,2:5)));
fprintf('%d\n',nonzeroCount(Img(1:4,6:9)));

S = load('S.mat').S;
fprintf('%d\n',nonzeroCount(S));

function output=nonzeroCount(S)
    nonzeroPixel = (S ~= 0);
    output = sum(nonzeroPixel,'all');
    
end