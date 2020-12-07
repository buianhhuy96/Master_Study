close all

% Constructs a multimedia reader object
vidReader = VideoReader('echo1.avi');
vidReader2 = VideoReader('echo2.avi');
% Initialize the optical flow object
opticFlow = opticalFlowLK('NoiseThreshold', 0.005);
f1 = figure;
f2 = figure;

while hasFrame(vidReader)
    % Read the rgb frame
    frameRGB  = readFrame(vidReader);
    % Convert rgb to grayscale
    frameGray = rgb2gray(frameRGB);
    % Compute optical flow
    flow = estimateFlow(opticFlow, frameGray);
    % Display rgb video frame with flow vectors
    figure(f1);
    imshow(frameRGB), title("echo1");
    hold on;
    plot(flow, 'DecimationFactor', [5 5], 'ScaleFactor', 10);
    drawnow;
    hold off;
end
while hasFrame(vidReader2)
    % Read the rgb frame
    frameRGB2  = readFrame(vidReader2);
    % Convert rgb to grayscale
    frameGray2 = rgb2gray(frameRGB2);
    % Compute optical flow
    flow = estimateFlow(opticFlow, frameGray2);
    % Display rgb video frame with flow vectors
    figure(f2);
    imshow(frameRGB2), title("echo2");;
    hold on;
    plot(flow, 'DecimationFactor', [5 5], 'ScaleFactor', 10);
    drawnow;
    hold off;
end
%%%%%%%%%%%%%%%%%%% Uncomment below for question c - e %%%%%%%%%%%%%%%%%
% 
% f3 = figure;
% while hasFrame(vidReader)
% % Read the rgb frame
%         frameRGB  = readFrame(vidReader);
%         % Convert rgb to grayscale
%         frameGray = (rgb2gray(frameRGB));        
%         frameGrayWithFilter = wiener2(frameGray,[3 3]);
%         % Compute optical flow
%         flow = estimateFlow(opticFlow, frameGray);
%         flow2 = estimateFlow(opticFlow, frameGrayWithFilter);
% 
%         % Display rgb video frame with flow vectors
%         figure(f3);
%         subplot(1,2,1), imshow(frameRGB);     
%         hold on;
%         plot(flow, 'DecimationFactor', [5 5], 'ScaleFactor', 10),title('Original');
%         drawnow;
%         hold off;
%         
%         subplot(1,2,2), imshow(frameRGB);
%         hold on;
%         plot(flow2, 'DecimationFactor', [5 5], 'ScaleFactor', 80),title('Filtered');
%         drawnow;
%         hold off;
% end