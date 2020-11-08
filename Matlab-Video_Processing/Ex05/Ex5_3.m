clear all;
close all;
I = imread("cameraman.tif");
noise_I = uint8(double(I) + (2*rand(size(I))-1)*10);

filter_I = directional_filtering(noise_I, 3);
filter_T = medfilt2(noise_I);
% Display images with Histogram Equalization and Contrast Stretching 
figure('Name','cameraman')
subplot(2,1,1), imshow(filter_T), title('Original');
subplot(2,1,2), imshow(uint8(filter_I)), title('Noise image');
% 
% % apply histogram matching into input images and display them.
% figure('Name','Histogram matching')
% subplot(2,3,1), imshow(B), title('spine - Original');
% subplot(2,3,2), imshow(myhistmatch(A,B)), title('spine - Histogram Matching');
% subplot(2,3,3), bar(myhist(myhistmatch(A,B)),'LineWidth',2),ylim([0 2000]), title('spine - Histogram');
% 
% subplot(2,3,4), imshow(C), title('church - Original');
% subplot(2,3,5), imshow(myhistmatch(A,C)), title('church - Histogram Matching');
% subplot(2,3,6), bar(myhist(myhistmatch(A,C)),'LineWidth',2),ylim([0 10000]), title('church - Histogram');
% 
% 
% function new_img = myhistmatch(ref_img,img)
% 	% cumulative distribution function for each gray level of image
%     p = 0;
%     cdf = zeros(1,256);
% 	% cumulative distribution function for each gray level of reference image
%     pr = 0;
%     cdfr = zeros(1,256);
%     for i = 0:255
%         p = p + double(sum(img(:)==i))/numel(img);
%         cdf(i+1) = p;
%         pr = pr + double(sum(ref_img(:)==i))/numel(ref_img);
%         cdfr(i+1) = pr;
%     end
% 	% Array to store new gray level over the old one
%     M = zeros(256,1,'uint8');
%     for idx = 1 : 256
%         [~,ind] = min(abs(cdf(idx) - cdfr));
%         M(idx) = ind-1;
%     end
%    
%     new_img = M(double(img)+1);
% end

function F_SP=directional_filtering(img, filter_size)
    filer=ones(filter_size);
    [r,c]=size(img);
    F_SP=zeros(r,c);
    for i=2:r-1
        for j=2:c-1
        flt=[img(i-1,j-1),img(i-1,j),img(i-1,j+1),img(i,j-1),img(i,j),img(i,j+1),img(i+1,j-1),img(i+1,j),img(i+1,j+1)];
        F_SP(i,j)=median(flt);
        end
    end
% figure;imshow(img,[]);
% figure;imshow(F_SP,[]);
end

%4.4. 
%	The purpose of applying histogram equalization in image is to produce an image with 
%	uniformly distributed gray level from completely black to completely white. In other words,
%	cumulative distribution function of result image will shape as linear line. 
%	Therefore, it is useless applying histogram equalization multiple times or applying 
%	to an image with linear cumulative distribution function


%	One option to improve Histogram Equalization (HE) is Adaptive Histogram Equalization, which applies 
%	HE into different area of the image not the whole.