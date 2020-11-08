clear all;
close all;
I = imread("cameraman.tif");

f = (-1)*ones(3);

% Apply log enhancement to image with different scale of c 
% Display results
figure('Name','Ex05.1')
subplot(2,2,1), imshow(I), title('Original');
f(2,2) = 8;
subplot(2,2,2), imshow(imfilter(I,f)), title('A = 8');
f(2,2) = 9;
subplot(2,2,3), imshow(imfilter(I,f)), title('A = 9');
f(2,2) = 9.7;
subplot(2,2,4), imshow(imfilter(I,f)), title('A = 9.7');
