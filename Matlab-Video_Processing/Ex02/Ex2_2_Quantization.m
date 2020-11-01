close all
clear all
I = imread("lena_face.png");

level = [128,64,32;16,8,4];
figure(1);
for i = 1:height(level)
    for j=1:length(level)
		% quantizing with 6 different level and show output images
        quantized_img = quantiz_with_level(I, level(i,j));
        subplot(2,3,(i-1)*length(level)+j), 
            imshow(quantized_img),
            title(string(level(i,j)) + '-level');
    end
end

%add noise to original image and quantize at 16-level
noise_I = imnoise(I,'speckle',0.005);
figure('Name','Added noise'),
imshow(quantiz_with_level(noise_I, 16));

function quantized_img=quantiz_with_level(img, level)
    reshaped_img = reshape(img,1,[]);
    step = double((max(reshaped_img)-min(reshaped_img))/level);
    % Define partition - array with uniformly distributed elements
    %                 start:"step"; end:"256-step"; step size: "step" 
    % This array contain quantization levels which decide the index of element
    % (of codebook array) that will go to the output
    partition = step:step:256-step;  
    
    % Define codebook - array with uniformly distributed elements
    %                 start:"step/2"; end:"256-step/2"; step size: "step" 
    % This array contain prescribed value of quantization output
    codebook = step/2:step:256-step/2;  
    [~,quantized_img] = quantiz(reshaped_img, partition, codebook);
    quantized_img = uint8(reshape(quantized_img,size(img)));
end