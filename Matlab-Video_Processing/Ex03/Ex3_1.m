clear all;
close all;
I = imread("wom1.png");

figure(1)
bar(myhist(I),'LineWidth',1), ylim([0 2500]);


I2 = imread("man8.png");
I3 = imread("wom1.png");
figure(2)
subplot(2,3,1), imshow(I2), title('man8');
subplot(2,3,2), imshow(ContrastStretch(I2)), title('constrast_stretched man8');
subplot(2,3,3), bar(myhist(ContrastStretch(I2)),'LineWidth',2),ylim([0 2000]), title('hist man8');

subplot(2,3,4), imshow(I3), title('mbaboon');
subplot(2,3,5), imshow(ContrastStretch(I3)), title('constrast_stretched mbaboon');
subplot(2,3,6), bar(myhist(ContrastStretch(I3)),'LineWidth',2),ylim([0 1000]), title('hist mbaboon');
%222 - 6
function histogram_y = myhist(img)
    for i= 0:255
        histogram_y(i+1) = length(img(img==i));
    end  
end
function new_img = ContrastStretch(img)
    [m,n] = size(img);
    reshaped_img = double(reshape(img,1,[]));
    min_element = min(reshaped_img);
    max_element = max(reshaped_img);
    diff = max_element - min_element;
    for i=1:length(reshaped_img)
        new_img(i) = (reshaped_img(i)-min_element)*255/diff;
    end
    
    % Reshape back to orginal size
    new_img = reshape(uint8(new_img),m,n);
end
