%%Deal svm problem using linprog
clc;
clear all;
load('ex6data1.mat');
x = zeros(size(X));
for i = 1:2
    x(:,i) = (X(:,i)-mean(X(:,i)))/std(X(:,i));
end 
[m,n] = size(x);
y = 2*y -1;
c = [0,0,ones(1,m)];
