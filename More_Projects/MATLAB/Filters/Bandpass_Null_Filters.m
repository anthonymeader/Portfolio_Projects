%%3.1 Test
%nullfilt = [1 -2*cos(.5*pi) 1];
%[h,w]=freqz(nullfilt,1,-pi:pi/1000:pi);
%figure(1);
%plot(w,abs(h))
%figure(2);
%plot(w,angle(h))

%3.1  Nulling Filters for Rejection
%a)
null_filter_0 = [1 -2*cos(.44*pi) 1];
null_filter_1 = [1 -2*cos(.7*pi) 1];
null_filter = firfilt(null_filter_0,null_filter_1);
[h,w] = freqz(null_filter, 1, -pi:pi/1000:pi);
figure(1);
plot(w,angle(h))
title("Phase of Null Filter");
title("Magnitude of Null Filter")
figure(2);
plot(w,angle(h))
%b)
n = 0:1:149;
xn = 5*cos(.3*pi.*n) + 22*cos(.44*pi.*n-(pi/3)) + 22*cos(.7*pi.*n-(pi/4));
xn_filter = firfilt(null_filter, xn);
figure(1);
subplot(211);
stem(xn(1:40));
title("Input Signal");
subplot(212);
stem(xn_filter(1:40));
title("Output Signal");

%Frequency is 1/20 = .05Hz
%Magnitude is 9.41406
figure(2);
x1 = 9.41406*cos(6*pi*(.05).*(n-2));
subplot(211);
stem(x1(5:40));
title("Output Signal with Independent Equation")
subplot(212);
stem(xn_filter(5:40));
title("Output Signal")

%3.2 Simple Bandpass Filter Design
%3.2 a)
L = 40;

nnew = 0:1:L-1;
xn = 5*cos(.3*pi.*n) + 22*cos(.44*pi.*n-(pi/3)) + 22*cos(.7*pi.*n-(pi/4));
hn = (2/L)*cos(.44*pi*nnew);
[h,w] = freqz(hn,1,-pi:pi/10000:pi);
figure(1);
plot(w/pi,abs(h));
title("Magnitude of Filter");
filtered = firfilt(hn,xn);
figure(2);
subplot(211);
plot(xn);
subplot(212);
plot(filtered);

%3.2 b)
pb = max(abs(h)*.707);
sb = max(abs(h)*.25);
passband= find(abs(h) >= pb);
stopband = find(abs(h) < sb);
figure(3);
subplot(211);
plot(w(passband)/pi,abs(h(passband)));
title("Passband L = 40");
subplot(212);
plot(w(stopband)/pi,abs(h(stopband)));
title("Stopband"); 
