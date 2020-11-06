clear all;
close all;
I = imread("moon.png");
I2 = imread("house.png");
I3 = imread("spine.jpg");
I4 = imread("church.png");

figure('Name','moon')
subplot(2,2,1), imshow(I), title('Original');
subplot(2,2,2), imshow(myhisteq(I)), title('Histeq');
subplot(2,2,3), imshow(myContrastStretch(I,0,255)), title('ConstrastStretch');
subplot(2,2,4), bar(myhist(I),'LineWidth',2),ylim([0 2000]), title('Histogram');

figure('Name','house')
subplot(2,2,1), imshow(I2), title('Original');
subplot(2,2,2), imshow(myhisteq(I2)), title('Histeq');
subplot(2,2,3), imshow(myContrastStretch(I2,0,255)), title('ConstrastStretch');
subplot(2,2,4), bar(myhist(I2),'LineWidth',2),ylim([0 2000]), title('Histogram');

figure('Name','spine')
subplot(2,2,1), imshow(I3), title('Original');
subplot(2,2,2), imshow(myhisteq(I3)), title('Histeq');
subplot(2,2,3), imshow(myContrastStretch(I3,0,255)), title('ConstrastStretch');
subplot(2,2,4), bar(myhist(I3),'LineWidth',2),ylim([0 2000]), title('Histogram');

figure('Name','church')
subplot(2,2,1), imshow(I4), title('Original');
subplot(2,2,2), imshow(myhisteq(I4)), title('Histeq');
subplot(2,2,3), imshow(myContrastStretch(I4,0,255)), title('ConstrastStretch');
subplot(2,2,4), bar(myhist(I4),'LineWidth',2),ylim([0 2000]), title('Histogram');

function new_img = myhisteq(img)
    p = 0;
    % cumulative distribution function for each gray level
    cdf = zeros(1,256);
    for i = 0:255
        % sum of probability of an occurrence of a gray level
        p = p + double(sum(img(:)==i))/numel(img);
        cdf(i+1) = p;
    end
    new_img = uint8(floor(255 * cdf(img+1)));
end

% ContrastStretch should be applied to image with highest gray level is
% not "hi", or lowest is not "lo". If there is ANY pixel with gray level at
% "hi" and "lo", ContrastStretch does nothing
function im1=myContrastStretch(im, lo, hi)
    im = double(im);
    immax = max(max(im));
    immin = min(min(im));
    im1 = uint8((im - immin)*(hi-lo)/(immax-immin)+lo);
end

function histogram_y = myhist(img)
    for i= 0:255
        histogram_y(i+1) = length(img(img==i));
    end  
end
