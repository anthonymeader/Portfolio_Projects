% Play the processed audio output

soundsc(double(audioOut), modelParams.audio.sampleFrequency)

audiowrite("mymodel_test_2_mysong.wav",double(audioOut),modelParams.audio.sampleFrequency);


%audiowrite("mymodel_test_2.wav",audioOut,modelParams.audio.sampleFrequency);
%song = audioOut ./ max(abs(audioOut)); %scale for audiowrite
%audiowrite("mymodel_test_3.wav",song,modelParams.audio.sampleFrequency);

song_1 = double(audioOut) ./ max(abs(double(audioOut))); %scale for audiowrite

audiowrite("mymodel_test_3_mysong.wav",song_1,modelParams.audio.sampleFrequency);

%figure(1);
%spectrogram(song(:,1),2500,2000,32768,modelParams.audio.sampleFrequency,'yaxis');
%figure(2);
%spectrogram()
%spectrogram(song_1(:,1),2500,2000,32768,modelParams.audio.sampleFrequency,'yaxis');




