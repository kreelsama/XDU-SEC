x = ones(1,8);

h1 = ones(1,10);
h2 = [1 2.5 2.5 1];

y1 = conv(x,h1);
y2 = conv(x,h2);

subplot(2,1,1);
stem(0:length(y1)-1, y1,'.');
title("h1");

subplot(2,1,2);
stem(0:length(y2)-1, y2,'.');
title("h2"); 