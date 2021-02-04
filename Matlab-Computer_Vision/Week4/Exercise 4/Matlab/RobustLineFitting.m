%% Load and plot points
load('points.mat','x','y');
figure;hold on;
plot(x,y,'kx');
axis equal

%% RANSAC parameters
% m is the number of data points
m=length(x);
% s is the size of the random sample
s=2;
% t is the inlier distance threshold
t=sqrt(3.84)*2;
% e is the expected outlier ratio
e=0.8;
% at least one random sample should be free from outliers with probability p 
p=0.999;
% required number of samples
N_estimated=log(1-p)/log(1-(1-e)^s);

%% RANSAC loop
% First initialize some variables
N=inf;
sample_count=0;
max_inliers=0;
best_line=zeros(3,1);

while(N>sample_count)
    % Pick two random samples
    id1=ceil(m*rand(1));  % sample id 1
    id2=ceil(m*rand(1));  % sample id 2
    if id1==id2 %if the same point is drawn twice, must draw again
        continue;
    end
    
    % Determine the line crossing the points with the cross product of the points (in homogeneous coordinates).
    % Also normalize the line by dividing each element by sqrt(a^2+b^2), where a and b are the line coefficients
    
    %%-your-code-starts-here%%
    point1= [x(id1); y(id1); 1];
    point2= [x(id2); y(id2); 1];
    line = cross(point1, point2); 
    line = line/rssq(line);
    %%-your-code-ends-here%%
    
    % Determine the inliers by finding the indices for the line and data
    % point dot products (absolute value) which are less than inlier threshold.
    
    %%-your-code-starts-here%%
    point=[x;y;repmat([1],1,130)];
    dist = dot(point,repmat(line,1,130),1);
    inliers = find(dist < t);
    %%-your-code-ends-here%%
    
    % keep the hypothesis giving most inliers so far
    inlier_count=length(inliers);
    if inlier_count>max_inliers
        best_line=line(:);
    end
    
    % update the estimate of the outlier ratio
    e=1-inlier_count/m;
    % update the estimate for the required number of samples
    N=log(1-p)/log(1-(1-e)^s);
    
    sample_count=sample_count+1;
end

% Least squares fitting to the inliers of the best hypothesis, i.e.
% find the inliers similarly as above but this time for the best line.

%%-your-code-starts-here%%
dist = dot(point,repmat(best_line,1,130),1);
indices = find(dist < t);
x_inliers = point(1,indices);
y_inliers = point(2,indices);
%%-your-code-ends-here%%

% Fit a line to the given points (non-homogeneous)
l=linefitlsq(x_inliers, y_inliers);

% plot the resulting line and the inliers
k=-l(1)/l(2);
b=-l(3)/l(2);
plot(1:100,k*[1:100]+b,'m-');
plot(x(inliers),y(inliers),'ro');
