clear all;
close all;

% Load image
I = (imread("lena.tiff"));
r = I(:, :, 1);
g = I(:, :, 2);
b = I(:, :, 3);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 10.2b %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
gb = imnoise(g,'gaussian');
Ib = cat(3,r,gb,b);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 10.2c %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

rc = imnoise(r,'gaussian');
gc = imnoise(g,'gaussian');
bc = imnoise(b,'gaussian');
Ic = cat(3,rc,gc,bc);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 10.2bd %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rId = channelNoiseFiltering(Ic,1);
gId = channelNoiseFiltering(Ic,2);
bId = channelNoiseFiltering(Ic,3);
hId = channelNoiseFiltering(rgb2hsi(Ic),1);
sId = channelNoiseFiltering(rgb2hsi(Ic),2);
iId = channelNoiseFiltering(rgb2hsi(Ic),3);


convert2HSIandDisplay(I,"a");

convert2HSIandDisplay(Ib,"b");

convert2HSIandDisplay(Ic,"c");
% Hue channel is worsen because noise in all channels results in color
% changes throughout all pixels, meanwhile hue represents pixel colors of image.
figure('Name', "all-noisy-channel image");
imshow((Ic)),title('all-noisy-channel image');

figure('Name', "d");
% Apply filter in R,G,B looks quite similar because 
subplot(2,3,1), imshow(rId),title('red');
subplot(2,3,2), imshow(gId),title('green');
subplot(2,3,3), imshow(bId),title('blue');
subplot(2,3,4), imshow(hsi2rgb(hId),[]),title('hue');
subplot(2,3,5), imshow(hsi2rgb(sId),[]),title('sat');
subplot(2,3,6), imshow(hsi2rgb(iId),[]),title('intensity');

function convert2HSIandDisplay(img,question_number)

	r = img(:, :, 1);
	g = img(:, :, 2);
	b = img(:, :, 3);
    hsvI = rgb2hsi(img);
	h = (hsvI(:,:,1));
    s = (hsvI(:,:,2));
    i = (hsvI(:,:,3));

	figure('Name', question_number);
	
	subplot(2,3,1), imshow(r),title('red');
	subplot(2,3,2), imshow(g),title('green');
	subplot(2,3,3), imshow(b),title('blue');
	subplot(2,3,4), imshow(h),title('hue');
	subplot(2,3,5), imshow(s),title('sat');
	subplot(2,3,6), imshow(i),title('intensity');
end

function output_img = channelNoiseFiltering(img,channel)    

    m = 5; n = 5;
    %output_images = uint8([]);
    double_channel = double(img(:,:,channel));
    % Arithmetic Mean filter:
    % Good at reducing noise in general. It leaves a little bit blurring
    % effect in the edge. However, it perform well in different situation
    img(:,:,channel) = (imfilter(double_channel,fspecial('average',[m n])));
	output_img = (img);
    
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
function rgb = hsi2rgb(hsi)
hsi = double(hsi);
%HSI2RGB Converts an HSI image to RGB.
%   RGB = HSI2RGB(HSI) converts an HSI image to RGB, where HSI is
%   assumed to be of class double with:  
%     hsi(:, :, 1) = hue image, assumed to be in the range
%                    [0, 1] by having been divided by 2*pi.
%     hsi(:, :, 2) = saturation image, in the range [0, 1].
%     hsi(:, :, 3) = intensity image, in the range [0, 1].
%
%   The components of the output image are:
%     rgb(:, :, 1) = red.
%     rgb(:, :, 2) = green.
%     rgb(:, :, 3) = blue.

%   Copyright 2002-2004 R. C. Gonzalez, R. E. Woods, & S. L. Eddins
%   Digital Image Processing Using MATLAB, Prentice-Hall, 2004
%   $Revision: 1.5 $  $Date: 2003/10/13 01:01:06 $

% Extract the individual HSI component images.
H = hsi(:, :, 1) * 2 * pi;
S = hsi(:, :, 2);
I = hsi(:, :, 3);

% Implement the conversion equations.
R = zeros(size(hsi, 1), size(hsi, 2));
G = zeros(size(hsi, 1), size(hsi, 2));
B = zeros(size(hsi, 1), size(hsi, 2));

% RG sector (0 <= H < 2*pi/3).
idx = find( (0 <= H) & (H < 2*pi/3));
B(idx) = I(idx) .* (1 - S(idx));
R(idx) = I(idx) .* (1 + S(idx) .* cos(H(idx)) ./ ...
                                          cos(pi/3 - H(idx)));
G(idx) = 3*I(idx) - (R(idx) + B(idx));

% BG sector (2*pi/3 <= H < 4*pi/3).
idx = find( (2*pi/3 <= H) & (H < 4*pi/3) );
R(idx) = I(idx) .* (1 - S(idx));
G(idx) = I(idx) .* (1 + S(idx) .* cos(H(idx) - 2*pi/3) ./ ...
                    cos(pi - H(idx)));
B(idx) = 3*I(idx) - (R(idx) + G(idx));

% BR sector.
idx = find( (4*pi/3 <= H) & (H <= 2*pi));
G(idx) = I(idx) .* (1 - S(idx));
B(idx) = I(idx) .* (1 + S(idx) .* cos(H(idx) - 4*pi/3) ./ ...
                                           cos(5*pi/3 - H(idx)));
R(idx) = 3*I(idx) - (G(idx) + B(idx));

% Combine all three results into an RGB image.  Clip to [0, 1] to
% compensate for floating-point arithmetic rounding effects.
rgb = cat(3, R, G, B);
rgb = (max(min(rgb, 1), 0));
end