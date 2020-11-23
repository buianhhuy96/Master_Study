clear all;
close all;

% Load image
I = (imread("fruits.jpg"));
I2 = (imread("festia.jpg"));

%%%%%%%%%%%%%%%%%%%%%%%% Histogram Equalization  %%%%%%%%%%%%%%%%%%%%%%%%%%

figure('Name', 'Histogram Equalization');
subplot(2,3,1), imshow(I),title('Original');
subplot(2,3,2), imshow(RGBhisteq(I)),title('RGB Histogram Equalization');
subplot(2,3,3), imshow(intensityeq(I)),title('HSV Histogram Equalization');

subplot(2,3,4), imshow(I2),title('Original');
subplot(2,3,5), imshow(RGBhisteq(I2)),title('RGB Histogram Equalization');
subplot(2,3,6), imshow(intensityeq(I2)),title('HSV Histogram Equalization');


function histeq_img = intensityeq(img)
    img = rgb2hsv(img);
	h = (img(:,:,1));
    s = (img(:,:,2));
    v = histeq(img(:,:,3));
    
    histeq_img = hsv2rgb(cat(3,h,s,v));
end


function histeq_img = RGBhisteq(img)
    r = histeq(img(:,:,1));
    g = histeq(img(:,:,2));
    b = histeq(img(:,:,3));
    histeq_img = cat(3,r,g,b);
end