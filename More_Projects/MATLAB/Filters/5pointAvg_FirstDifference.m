close all
clear all
load lab6dat.mat


%3.2 Implementation of a 5-point Averager
%a
h5avg = (1/5)*ones(1,5);
v1 = firfilt(x1,h5avg);
figure(1);
subplot(211)
stem(x1);
title("5 Point Average");
xlim([0, length(x1)]);
xlabel("Time Index (n)");
ylabel("X1(n)")
subplot(212)
stem(v1);
xlim([0, length(v1)]);
xlabel("Time Index (n)");
ylabel("V1(n)")
%b
[h,w]=freqz(h5avg,1,(-pi:pi/200:pi));
figure(2);
plot(w,abs(h)); %looks like a highpass
xlabel("Time Index (w)");
ylabel("Magnitude");
figure(3);
plot(w,angle(h));
xlabel("Time Index (w)");
ylabel("Phase");

%3.3 First Difference
hd = [1 -1];
v2 = firfilt(x1,hd);
figure(4);
subplot(211)
stem(x1);
xlim([0, length(x1)]);
title("First Difference System");
xlabel("Time Index (n)");
ylabel("X1(n)")
subplot(212)
stem(v2);
xlim([0, length(v1)]);
xlabel("Time Index (n)");
ylabel("V2(n)")
[h,w] = freqz(hd,1,(-pi:pi/200:pi));
figure(5);
plot(w,abs(h));
xlabel("Time Index (w)");
ylabel("Magnitude");

%3.4 Implementation of the Figure 1 Cascade
%a
h5avg = (1/5)*ones(1,5);
v1 = firfilt(x1,h5avg);
hd = [1 -1];
y1 = firfilt(v1,hd);
figure(6);
subplot(211)
stem(v1);
title("Filter x1 w/ 5 Point Average")
xlabel("Time Index (n)");
ylabel("V1(n)")
xlim([0, length(x1)]);
subplot(212)
stem(y1);
title("Filter v1 w/ First Difference")
xlabel("Time Index (n)");
ylabel("Y1(n)")
xlim([0, length(v1)]);
%b
[h,w] = freqz(hd,1,(-pi:pi/200:pi));
figure(7);
plot(w,abs(h));
xlabel("Time Index (w)");
ylabel("Magnitude");
figure(8);
plot(w,angle(h));
xlabel("Time Index (w)");
ylabel("Phase");

%3.5 Implementation of the Figure 2 Cascade
%a
hd = [1 -1];
v2 = firfilt(x1,hd);
h5avg = (1/5)*ones(1,5);
y2 = firfilt(v2,h5avg);
figure(9);
subplot(211)
stem(v2);
title("Filter x1 w/ First Difference")
xlabel("Time Index (n)");
ylabel("V2(n)")
xlim([0, length(v2)]);

subplot(212)
stem(y2);
title("Filter v2 w/ 5 Point Average")
xlabel("Time Index (n)");
ylabel("Y2(n)")
xlim([0, length(y2)]);
%b
[h,w] = freqz(h5avg,1,(-pi:pi/200:pi));
figure(10);
plot(w,abs(h));
xlabel("Time Index (w)");
ylabel("Magnitude");
figure(11);
plot(w,angle(h));
xlabel("Time Index (w)");
ylabel("Phase");

dif = sum((y1-y2) .* (y1-y2));
res = y1-y2;
