A = [1, -0.9];
B = [0.05, 0.05];
n = 1:50;
zi = filtic(B,A,0);

x = [1 zeros(1,49)];
[yn, ~] = filter(B,A,x,zi);
subplot(3,1,1);
stem(n, yn,'.');
title("Pulse");
grid on;

x = [ones(1,8),zeros(1,42)];
[yn, ~] = filter(B,A,x,zi);
subplot(3,1,2);
stem(n, yn,'.');
title("Rectangle");
grid on;

x = ones(1,50);
[yn, ~] = filter(B,A,x, zi);
subplot(3,1,3);
stem(n, yn,'.');
title("Unary pulse");
grid on;