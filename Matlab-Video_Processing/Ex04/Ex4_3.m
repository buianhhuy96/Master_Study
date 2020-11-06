clear all
close all;
A = imread("corel.png");
B = imread("spine.jpg");
C = imread("church.png");

figure('Name','Reference')
subplot(1,2,1), imshow(A), title('corel - Original');
subplot(1,2,2), bar(myhist(A),'LineWidth',2),ylim([0 2000]), title('corel - Histogram');

figure('Name','Histogram matching')
subplot(2,3,1), imshow(B), title('spine - Original');
subplot(2,3,2), imshow(myhistmatch(A,B)), title('spine - Histogram Matching');
subplot(2,3,3), bar(myhist(myhistmatch(A,B)),'LineWidth',2),ylim([0 2000]), title('spine - Histogram');

subplot(2,3,4), imshow(C), title('church - Original');
subplot(2,3,5), imshow(myhistmatch(A,C)), title('church - Histogram Matching');
subplot(2,3,6), bar(myhist(myhistmatch(A,C)),'LineWidth',2),ylim([0 10000]), title('church - Histogram');


function new_img = myhistmatch(ref_img,img)
	% cumulative distribution function for each gray level of image
    p = 0;
    cdf = zeros(1,256);
	% cumulative distribution function for each gray level of reference image
    pr = 0;
    cdfr = zeros(1,256);
    for i = 0:255
        p = p + double(sum(img(:)==i))/numel(img);
        cdf(i+1) = p;
        pr = pr + double(sum(ref_img(:)==i))/numel(ref_img);
        cdfr(i+1) = pr;
    end
    M = zeros(256,1,'uint8');
    for idx = 1 : 256
        [~,ind] = min(abs(cdf(idx) - cdfr));
        M(idx) = ind-1;
    end
   
    new_img = M(double(img)+1);
end

function histogram_y = myhist(img)
    for i= 0:255
        histogram_y(i+1) = length(img(img==i));
    end  
end

%4.4. 
%The purpose of applying histogram equalization in image is to produce an image with 
%uniformly distributed gray level from completely black to completely white. In other words,
%cumulative distribution function of result image will shape as linear line. 
%Therefore, it is useless applying histogram equalization multiple times or applying 
%to an image with linear cumulative distribution function
