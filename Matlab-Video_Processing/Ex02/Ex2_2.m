clear all;
close all;
I = imread("lena_face.png");

quantizer_level = 8;
subplot(2,3,1), imshow(image_quantization(I, 128)),title("128-level");
subplot(2,3,2), imshow(image_quantization(I,  64)),title("64-level");
subplot(2,3,3), imshow(image_quantization(I,  32)),title("32-level");
subplot(2,3,4), imshow(image_quantization(I,  16)),title("16-level");
subplot(2,3,5), imshow(image_quantization(I,   8)),title("8-level");
subplot(2,3,6), imshow(image_quantization(I,   4)),title("4-level");

noised_I = imnoise(I,'speckle',0.01);
figure, imshow(image_quantization(noised_I,  16));

function new_img = image_quantization(img, level)
    [m,n] = size(img);
    % Reshape to 1-D array for quantization
    img_reshaped = double(reshape(img,1,[]));
    
    % Define step value/minimum value of discretized array 
    step = (max(img_reshaped)-min(img_reshaped))/level;

    % Defind a sequence with fixed step size
    % In this case, start    : "min(img_reshaped)+step",
    %               end      : "max(img_reshaped)-step"
    %               step size: "step"
    partition = (min(img_reshaped)+step):step:(max(img_reshaped)-step);

    % Define a sequence with fixed step size
    % In this case, start    : "min(img_reshaped)+step/2"
    %               end      : "max(img_reshaped)-step/2"
    %               step size: "step"
    codebook = (min(img_reshaped)+step/2):step:(max(img_reshaped)-step/2);

    % Quantize img with defined partition and codebook
    [~,new_img] = quantiz(img_reshaped, partition, codebook);
    
    % Reshape back to orginal size
    new_img = reshape(uint8(new_img),m,n);
end
