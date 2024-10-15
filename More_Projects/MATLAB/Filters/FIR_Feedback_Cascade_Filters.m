%FIR Filter
b = [1 0 0 1]; 
a = [1 0 0 0];
[h,w] = freqz(b,a,-pi:pi/100:pi);
figure(1);
plot(abs(h))
title("FIR Filter");

%Feedback Filter
b1 = [1 0 0 0]; 
a1 = [1 0 0 .81];
[h1,w1] = freqz(b1,a1,-pi:pi/100:pi);
figure(2);
plot(abs(h1))
title("Feedback Filter");

%Cascaded Filter
[h2,w2] = freqz(b,a1,-pi:pi/100:pi);
figure(3);
plot(abs(h2))
title("Cascade Filter");




