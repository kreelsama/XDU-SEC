A = [1, -1.8237, 0.9801];
B = [1/100.49, 1/100.49];

xn = ones(1,200);
zi = filtic(B,A,0);
yn = filter(B,A,xn,zi);
subplot(2,1,1);
stem(0:length(yn)-1, yn, '.');

n = 0:200;
xn = sin(0.014*n)+sin(0.4*n);
zi = filtic(B,A,0);
yn = filter(B,A,xn, zi);
subplot(2,1,2);
stem(0:length(yn)-1,yn,'.');
grid on;