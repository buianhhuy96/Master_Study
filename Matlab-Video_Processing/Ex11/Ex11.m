clear all;
close all;

% Load image
I2 = (load("yuvdata.mat"));
rows = I2.rows;
cols = I2.cols;
sizeI = I2.rows*I2.cols;
% Y = reshape(imresize(I2.yy,[sizeI 1]), [I2.rows I2.cols]);
% U = reshape(imresize(I2.uu,[sizeI 1]), [I2.rows I2.cols]);
% V = reshape(imresize(I2.vv,[sizeI 1]), [I2.rows I2.cols]);
Y = reshape((I2.yy),cols,rows);
U = reshape((I2.uu),cols/2,rows/2);
V = reshape((I2.vv),cols/2,rows/2);

U = repelem(U,2,2);
V = repelem(V,2,2);
f1 = figure('Name', 'YUV');
subplot(1,3,1), imshow(uint8(Y')),title('Y');
subplot(1,3,2), imshow(uint8(U')),title('U');
subplot(1,3,3), imshow(uint8(V')),title('V');
U = U - 127;
V = V - 127;
YUV=cat(2,Y(:),U(:),V(:));

YuvToRgb=[1  0          1.402;
          1 -0.34413 -0.71414; 
          1  1.772         0];
      
shaped_RGB = (YuvToRgb*YUV');
r = reshape(shaped_RGB(1,:),[I2.cols I2.rows]);
g = reshape(shaped_RGB(2,:),[I2.cols I2.rows]);
b = reshape(shaped_RGB(3,:),[I2.cols I2.rows]);
RGB = cat(3,r',g',b');
f2 = figure('Name', 'RGB');
imshow(uint8(RGB)),title('RGB');


