close all;
clear all;
[xnew,fs]=audioread("bluesSampleCleanShort.wav");
xx = xnew(1:250000,1); %shorten audio

%Control Params, Gain set between 0-1
gain_1 = 1;
gain_2 = 0;
gain_3 = 0;
gain_4 = 0;
gain_5 = 1;

%Convert song to frequency domain
N = length(xx);
Yn = fft(xx);
fn = (0:N-1)*(fs/N);
Y = fftshift(Yn); %Shift 0Hz to center 
fr = (-N/2:N/2-1)*fs/(N);

%Set frequency bands 
freq_range_1 = [0 300]; %bass
fneg_range_1 = [0 -300];

freq_range_2 = [300 2000]; %low mids
fneg_range_2 = [-300 -2000];

freq_range_3 = [2000 4000]; %high mids
fneg_range_3 = [-2000 -4000];

freq_range_4 = [4000 6000]; %presence
fneg_range_4 = [-4000 -6000];

freq_range_5 = [6000 30000]; %brilliance
fneg_range_5 = [-6000 -30000];

idx_range_1 = find(fr >= freq_range_1(1) & fr <= freq_range_1(2)); %find frequencies index 0 to 300Hz
idx_range_2 = find(fr >= freq_range_2(1) & fr <= freq_range_2(2)); %find frequencies index 300 to 2000Hz
idx_range_3 = find(fr >= freq_range_3(1) & fr <= freq_range_3(2)); %find frequencies index 2000 to 4000Hz
idx_range_4 = find(fr >= freq_range_4(1) & fr <= freq_range_4(2)); %find frequencies index 4000 to 6000Hz
idx_range_5 = find(fr >= freq_range_5(1) & fr <= freq_range_5(2)); %find frequencies index 6000 to 2000Hz

neg_range_1 = find(fr >= fneg_range_1(2) & fr <= fneg_range_1(1)); %find frequencies index 0 to -300Hz
neg_range_2 = find(fr >= fneg_range_2(2) & fr <= fneg_range_2(1)); %find frequencies index -300 to -2000Hz
neg_range_3 = find(fr >= fneg_range_3(2) & fr <= fneg_range_3(1)); %find frequencies index -2000 to -4000Hz
neg_range_4 = find(fr >= fneg_range_4(2) & fr <= fneg_range_4(1)); %find frequencies index -4000 to -6000Hz
neg_range_5 = find(fr >= fneg_range_5(2) & fr <= fneg_range_5(1)); %find frequencies index -6000 to -20000Hz

%Apply Gain to frequencies at determined index
Y_mod=Y;
Y_mod(idx_range_1) = Y_mod(idx_range_1) * gain_1; 
Y_mod(idx_range_2) = Y_mod(idx_range_2) * gain_2;
Y_mod(idx_range_3) = Y_mod(idx_range_3) * gain_3;
Y_mod(idx_range_4) = Y_mod(idx_range_4) * gain_4;
Y_mod(idx_range_5) = Y_mod(idx_range_5) * gain_5;

Y_mod(neg_range_1) = Y_mod(neg_range_1) * gain_1;
Y_mod(neg_range_2) = Y_mod(neg_range_2) * gain_2;
Y_mod(neg_range_3) = Y_mod(neg_range_3) * gain_3;
Y_mod(neg_range_4) = Y_mod(neg_range_4) * gain_4;
Y_mod(neg_range_5) = Y_mod(neg_range_5) * gain_5;

Y_mod_nc = ifftshift(Y_mod); %frequency not shifted
song = ifft(Y_mod_nc); %song back to time domain
song = real(song);
song = song ./ max(abs(song)); %scale for audiowrite




%audiowrite("soundexample1_e.wav",song,fs);
spectrogram(song(:,1),2500,2000,32768,fs,'yaxis');
soundsc(song,fs);

% figure(2);
% hold on
% plot(500000:1000000,song)
% xlabel('Seconds') 
% ylabel('Amplitude')
% title("Presence & Brilliance Boost");
% hold off



% audiowrite("midrange_boosted.wav",song,fs);
% 
 % figure(1);
 % hold on
 % plot(fn,abs(Y_mod_nc));
 % xlabel("Frequency");
 % ylabel("Magnitude");
 % hold off;
% % 
%  figure(2);
%  hold on
%  plot(fr,abs(Y_mod));
%  xlabel("Frequency");
%  ylabel("Magnitude");
%  hold off
% 
% 
% 
%  figure(3);
% % figure(4);
% spectrogram(xx(:,1),2500,2000,32768,fs,'yaxis');
% title("Spectrogram of Original Song");

%Bass 60-250Hz
%Low Mids 250 - 2Khz
%High Mids 2kHz - 4Khz
%Presence 4Khz - 6Khz
%Brilliance 6KHz - 16KHz
%Potentiometers to Control Gain
%Switch or button to select frequency range to adjust
