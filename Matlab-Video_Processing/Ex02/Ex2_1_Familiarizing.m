close all
clear all
I = imread("peppers.png");
A = (0:255);

% Define quantization steps
step = 256/4; 
% QA1 returns values from A rounded to nearest multiple of step
QA1 = quant(A, step);  

% Define partition - array with uniformly distributed elements
%                 start:"step"; end:"256-step"; step size: "step" 
% This array contain quantization levels which decide the index of element
% (of codebook array) that will go to the output
partition = step:step:256-step;

% Define codebook - array with uniformly distributed elements
%                 start:"step/2"; end:"256-step/2"; step size: "step" 
% This array contain prescribed value of quantization output
codebook = step/2:step:256-step/2;

% Quantization A array: Map A's elements with codebook's elements based on
% the indices which retrieved when comparing A's elements with partition's
% elements
[indx,QA2] = quantiz(A, partition, codebook); 

fprintf("QA1: [ ");
fprintf("%d ", unique(QA1));
fprintf("]\n");
fprintf("QA2: [ ");
fprintf("%d ",unique(QA2));
fprintf("]\n");