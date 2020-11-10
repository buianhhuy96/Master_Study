clear all;
close all;

I1 = 0.5*ones(128);
I2 = zeros(128);
I2(55:74,55:74) = ones(20);

x = linspace(0,1,128);
[I3,~] = meshgrid(x);  

y = x;
[X,Y] = meshgrid(x);

F = sin(Y*pi).*sin(X*pi);
surf(x,y,F);
%imshow(I2,[]);
