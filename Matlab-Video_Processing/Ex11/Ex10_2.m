clear all;
close all;

% Load image
I = (imread("lena.tiff"));
ycbcrI = rgb2ycbcr(I);
Y = ycbcrI(:,:,1);
Cb =  ycbcrI(:,:,2);
Cv =  ycbcrI(:,:,3);
%%%%%%%%%%%%% 4:2:2 %%%%%%%%%%%%%
Cb422 = Cb(:,(1:2:end));
Cv422 = Cv(:,(1:2:end));
RGB422 = resizeAndConcat(Y,Cb422,Cv422,size(Y));

%%%%%%%%%%%%% 4:1:1 %%%%%%%%%%%%%
Cb411 = Cb(:,(1:4:end));
Cv411 = Cv(:,(1:4:end));
RGB411 = resizeAndConcat(Y,Cb411,Cv411,size(Y));

%%%%%%%%%%%%% 4:2:0 %%%%%%%%%%%%%
Cb420 = Cb((1:2:end),(1:2:end));
Cv420 = Cv((1:2:end),(1:2:end));
RGB420 = resizeAndConcat(Y,Cb420,Cv420,size(Y));

%%%%%%%%%%%%% 4:2:0 with luminance %%%%%%%%%%%%%
Y420 = Y((1:2:end),(1:2:end));
RGB420_luminance = resizeAndConcat(Y420,Cb420,Cv420,size(Y));

f1 = figure('Name', '11.2');
imshow(uint8(I)),title('Original');

f2 = figure('Name', '11.2');
err422 = immse(RGB422, I);
err411 = immse(RGB411, I);
err420 = immse(RGB420, I);
err420_luminance = immse(RGB420_luminance, I);
subplot(2,2,1), imshow(uint8(RGB422)),title('422 - err: '+ string(err422));
subplot(2,2,2), imshow(uint8(RGB411)),title('411 - err: '+ string(err411));
subplot(2,2,3), imshow(uint8(RGB420)),title('420 - err: '+ string(err420));
subplot(2,2,4), imshow(uint8(RGB420_luminance)),title('420 with luminance - err: '+...
                                                string(err420_luminance));
function RGB = resizeAndConcat(Y,Cb,Cv,size)

	Y_resize = imresize(Y,size);
	Cb_resize = imresize(Cb,size);
    Cv_resize = imresize(Cv,size);
    YCbCv = cat(3,Y_resize,Cb_resize,Cv_resize);
    RGB = ycbcr2rgb(YCbCv);
end
