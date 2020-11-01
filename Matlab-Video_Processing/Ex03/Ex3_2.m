clear all;
close all;
I = imread("mbaboon.bmp");
fun1 = @(block_struct) block_struct.data(2,2);
fun2 = @(block_struct) block_struct.data(1,1);
fun3 = @(block_struct) mean2(block_struct.data);

 
subplot(1,3,1), imshow(blockproc(I,[4 4],fun1)), title('man8');
subplot(1,3,2), imshow(blockproc(I,[4 4],fun2)), title('constrast_stretched man8');
subplot(1,3,3), imshow(uint8(blockproc(I,[4 4],fun3))), title('hist man8');
