clear all;
close all;

% Load image
I = (imread("lena.tiff"));
r = I(:, :, 1);
g = I(:, :, 2);
b = I(:, :, 3);
convert2HSIandDisplay(I,"a");

gb = imnoise(g,'gaussian');
Ib = cat(3,r,gb,b);

convert2HSIandDisplay(Ib,"b");

rc = imnoise(r,'gaussian');
gc = imnoise(g,'gaussian');
bc = imnoise(b,'gaussian');
Ic = cat(3,rc,gc,bc);
convert2HSIandDisplay(Ib,"c");

function convert2HSIandDisplay(img,question)

r = img(:, :, 1);
g = img(:, :, 2);
b = img(:, :, 3);
    hsvI = rgb2hsi(img);
	h = (hsvI(:,:,1));
    s = (hsvI(:,:,2));
    i = (hsvI(:,:,3));

figure('Name', question);

subplot(2,3,1), imshow(r),title('red');
subplot(2,3,2), imshow(g),title('green');
subplot(2,3,3), imshow(b),title('blue');
subplot(2,3,4), imshow(h),title('hue');
subplot(2,3,5), imshow(s),title('sat');
subplot(2,3,6), imshow(i),title('intensity');
end

function output_img = sliceCube(img,dist)

    r = double(img(:,:,1));
    g = double(img(:,:,2));
    b = double(img(:,:,3));
    chosen_color = double(impixel(img));
    
    fprintf('\n The chosen color is %0.15f\n', chosen_color);
    color_dist = abs(r - chosen_color(1)) + ...
                 abs(g - chosen_color(2)) + ...
                 abs(b - chosen_color(3));
    mask = (color_dist < dist);  
    r_filtered = (double(~mask) .* 127) + double(mask) .* r;     
    g_filtered = (double(~mask) .* 127) + double(mask) .* g; 
    b_filtered = (double(~mask) .* 127) + double(mask) .* b; 
    output_img = cat(3,r_filtered,g_filtered,b_filtered);
    
end
function hsi = rgb2hsi(rgb)
%RGB2HSI Converts an RGB image to HSI.
%   HSI = RGB2HSI(RGB) converts an RGB image to HSI. The input image
%   is assumed to be of size M-by-N-by-3, where the third dimension
%   accounts for three image planes: red, green, and blue, in that
%   order. If all RGB component images are equal, the HSI conversion
%   is undefined. The input image can be of class double (with values
%   in the range [0, 1]), uint8, or uint16. 
%
%   The output image, HSI, is of class double, where:
%     hsi(:, :, 1) = hue image normalized to the range [0, 1] by
%                    dividing all angle values by 2*pi. 
%     hsi(:, :, 2) = saturation image, in the range [0, 1].
%     hsi(:, :, 3) = intensity image, in the range [0, 1].

%   Copyright 2002-2004 R. C. Gonzalez, R. E. Woods, & S. L. Eddins
%   Digital Image Processing Using MATLAB, Prentice-Hall, 2004
%   $Revision: 1.5 $  $Date: 2005/01/18 13:44:59 $

% Extract the individual component images.
rgb = im2double(rgb);
r = rgb(:, :, 1);
g = rgb(:, :, 2);
b = rgb(:, :, 3);

% Implement the conversion equations.
num = 0.5*((r - g) + (r - b));
den = sqrt((r - g).^2 + (r - b).*(g - b));
theta = acos(num./(den + eps));

H = theta;
H(b > g) = 2*pi - H(b > g);
H = H/(2*pi);

num = min(min(r, g), b);
den = r + g + b;
den(den == 0) = eps;
S = 1 - 3.* num./den;

H(S == 0) = 0;

I = (r + g + b)/3;

% Combine all three results into an hsi image.
hsi = cat(3, H, S, I);
end
