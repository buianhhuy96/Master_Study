clear all;
close all;
I = imread("cameraman.tif");
noise_I = uint8(double(I) + (2*rand(size(I))-1)*10);
% Display images with Histogram Equalization and Contrast Stretching 
figure('Name','cameraman')
subplot(2,1,1), imshow(I), title('Original');
subplot(2,1,2), imshow(noise_I), title('Noise image');

filter_I = directional_filtering(noise_I, 1);
% Display images with Histogram Equalization and Contrast Stretching 
figure('Name','directional_filtering 3x3')
subplot(2,2,1), imshow(uint8(filter_I(:,:,1))), title('0 degree');
subplot(2,2,2), imshow(uint8(filter_I(:,:,2))), title('45 degree');
subplot(2,2,3), imshow(uint8(filter_I(:,:,3))), title('90 degree');
subplot(2,2,4), imshow(uint8(filter_I(:,:,4))), title('135 degree');

figure('Name','sum 3x3')
imshow(uint8((filter_I(:,:,1)+ filter_I(:,:,2)+filter_I(:,:,3)+filter_I(:,:,4))/4)), title('0 degree');
% figure('Name','house')
% subplot(2,2,1), imshow(I2), title('Original');
% subplot(2,2,2), imshow(myhisteq(I2)), title('Histeq');
% subplot(2,2,3), imshow(myContrastStretch(I2,0,255)), title('ContrastStretch');
% subplot(2,2,4), bar(myhist(I2),'LineWidth',2),ylim([0 2000]), title('Histogram');
% 
% figure('Name','spine')
% subplot(2,2,1), imshow(I3), title('Original');
% subplot(2,2,2), imshow(myhisteq(I3)), title('Histeq');
% subplot(2,2,3), imshow(myContrastStretch(I3,0,255)), title('ContrastStretch');
% subplot(2,2,4), bar(myhist(I3),'LineWidth',2),ylim([0 2000]), title('Histogram');
% 
% figure('Name','church')
% subplot(2,2,1), imshow(I4), title('Original');
% subplot(2,2,2), imshow(myhisteq(I4)), title('Histeq');
% subplot(2,2,3), imshow(myContrastStretch(I4,0,255)), title('ContrastStretch');
% subplot(2,2,4), bar(myhist(I4),'LineWidth',2),ylim([0 2000]), title('Histogram');
% 
% % Histogram Equalization modifies the gray level of each pixel based on how many pixel
% % has the same or lower its gray level compared to the total pixels of the image.
% function new_img = myhisteq(img)
%     p = 0;
%     % cumulative distribution function for each gray level
%     cdf = zeros(1,256);
%     for i = 0:255
%         % sum of probability of an occurrence of a gray level
%         p = p + double(sum(img(:)==i))/numel(img);
%         cdf(i+1) = p;
%     end
%     new_img = uint8(floor(255 * cdf(img+1)));
% end
%  
% % Contrast Stretching modifies the gray level of each pixel based on gray level itself 
% % in comparision to the highest and lowest gray level of the image.
% 
% % ContrastStretch is only effective to image with highest and lowest gray level is
% % somewhere in between 255 and 0, "hi" and "lo" in this function. 
% % If there is ANY pixel with gray level at % "hi" and "lo", ContrastStretch does nothing
function output_images=directional_filtering(img, filter_size)
    filer_0=zeros(filter_size);
    filer_0(uint8(filter_size/2+0.5),:) = 1/filter_size;
    
    filer_45 = rot90(eye(filter_size))/filter_size;
    
    filer_90=zeros(filter_size);
    filer_90(:,uint8(filter_size/2+0.5)) = 1/filter_size;
    
    filer_135 = (eye(filter_size))/filter_size;
    
    output_images = [];
    output_images(:,:,1) = imfilter(img,filer_0);
    output_images(:,:,2) = imfilter(img,filer_45);
    output_images(:,:,3) = imfilter(img,filer_90);
    output_images(:,:,4) = imfilter(img,filer_135);
end
% 
% function histogram_y = myhist(img)
%     for i= 0:255
%         histogram_y(i+1) = length(img(img==i));
%     end  
% end
