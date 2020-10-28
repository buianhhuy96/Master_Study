close all
I = imread("lena_face.png");
I1 = rgb2gray(I);

r = I(:,:,1);
g = I(:,:,2);
b = I(:,:,3);

I2 = I;
I2(:,:,2)= I2(:,:,2)+ 50;


I3 = cat(3,b,g,r);

title1='Original';
title2='Grayscale';
title3='Added-Green'; %it's like a green filter applied to the original image
title4='BGR';


% figure('Name',title1),imshow(I);
% figure('Name',title2),imshow(I1);
% figure('Name','Red Component'),imshow(r);
% figure('Name',title3),imshow(I2);
% figure('Name',title4),imshow(I3);

subplot(2,2,1), imshow(I),title(title1);
subplot(2,2,2), imshow(I1),title(title2);
subplot(2,2,3), imshow(I2),title(title3);
subplot(2,2,4), imshow(I3),title(title4);