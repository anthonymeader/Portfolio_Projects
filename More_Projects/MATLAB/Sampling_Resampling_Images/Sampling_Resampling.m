%3c, 3.1a-e, 3.2c

%3 Sampling and Resampling Images
%b
load dog.mat
xx = dd;
xs = imsample(xx,4);
show_img(dd,1);
show_img(xs,2);

load zone.mat
xzone = imsample(zone,2);
show_img(zone,3);
show_img(xzone,4);