close all
clear all

A = uint8(ones(400));
B = uint8(ones(400));

% Assign color to A
A(:,:) = 63;
A(120:280,120:280) = 127;

% Assign color to B
B(:,:) = 223;
B(120:280,120:280) = 185;


imshow(cat(2,A,B));