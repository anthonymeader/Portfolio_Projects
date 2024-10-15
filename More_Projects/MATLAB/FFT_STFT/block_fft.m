function xx=block_fft(in,len,thresh)
% xx = block_fft(in, len)
% Calculates windowed FFT (length len, with 50% overlap),
%  then inverse FFT with overlap-add.  If no processing is inserted
%  into the function below, the output should be essentially identical
%  to the input.
%
% in  = input data vector
% len = desired fft length (e.g., 1024)
% xx  = output data vector

in=in(:);

% Create output buffer for overlap add
xx=zeros(1,length(in));

% Create length “len” raised cosine window function
wind=0.5*(1-cos(2*pi*(0:len-1)/len));

% Stride through the input with 50% overlap (len/2), 
% calculate windowed length “len” FFT of each block,
% then inverse transform and overlap-add.

for i=1:len/2:(length(in)-len)

    ff=fft(wind.*in(i:(i+len-1))',len);
    
 ff = ff * 2.5;%  tresh = 6
% User processing of FFT data goes here…
%
   fft_mag = abs(ff);  % use the magnitude of the complex FFT
   %sum(fft_mag)/length(fft_mag)
   ff = ff.*(fft_mag > thresh);  % create a mask: 1 if the FFT magnitude > threshold, zero otherwise
%
% ...end of processing.
%

% Overlap add the output inverse transforms

xx(i:(i+len-1))= xx(i:(i+len-1))+ ifft(ff,len);

end

