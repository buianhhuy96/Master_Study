clear all;
close all;

I1 = 0.5*ones(128);

imageDFT(I1);

I2 = zeros(128);
I2(55:74,55:74) = ones(20);

imageDFT(I2);
x = linspace(0,1,128);
[I3,~] = meshgrid(x);  
imageDFT(I3);


y = x;
[X,Y] = meshgrid(x);
k=4
I5 = 0.5*cos(Y*k*2*pi).*cos(X*k*2*pi)+0.5;
imageDFT(I5);
%surf(x,y,F);
%imshow(I2,[]);


function filteredImg = imageDFT(img)
    FFT = fftshift(fft2(img))
    imshow(log(abs(FFT)+0.0001),[])
    
end