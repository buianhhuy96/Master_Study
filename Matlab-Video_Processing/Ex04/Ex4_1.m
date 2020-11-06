clear all;
close all;
I = imread("university.png");

figure(1)
imshow(I), title('Original');


figure(2)
subplot(2,3,1), imshow(log_enhance(I,10)), title('c = 10');
subplot(2,3,2), imshow(log_enhance(I,20)), title('c = 20');
subplot(2,3,3), imshow(log_enhance(I,30)), title('c = 30');

subplot(2,3,4), imshow(log_enhance(I,40)), title('c = 40');
subplot(2,3,5), imshow(log_enhance(I,50)), title('c = 50');
subplot(2,3,6), imshow(log_enhance(I,60)), title('c = 60');

figure(3)
subplot(2,3,1), imshow(power_law_enhance(I,2,1)), title('c = 2 y=1');
subplot(2,3,2), imshow(power_law_enhance(I,4,1.2)), title('c = 4 y=1.2');
subplot(2,3,3), imshow(power_law_enhance(I,4,1.5)), title('c = 4 y=1.5');
subplot(2,3,4), imshow(power_law_enhance(I,2,1.8)), title('c = 2 y=1.8');
subplot(2,3,5), imshow(power_law_enhance(I,2,2)), title('c = 2 y=2.0');
subplot(2,3,6), imshow(power_law_enhance(I,2,2.2)), title('c = 2 y=2.2');
%222 - 6
function enhanced_img = log_enhance(img, c)
    enhanced_img = uint8(c*log(1+double(img(:,:))));  
end


function enhanced_img = power_law_enhance(img, c, y)
    enhanced_img = uint8(c*((double(img(:,:)).^y)));  
end