% SPDX-License-Identifier: MIT
% Copyright (c) 2022 Ross K. Snider  All rights reserved.
%--------------------------------------------------------------------------
% Description:  Matlab Function to create the frequency domain filters
%               for the FFTAnalysisSynthesis  Simulink model. 
%--------------------------------------------------------------------------
% Authors:      Ross K. Snider
% Company:      Montana State University
% Create Date:  May 19, 2022
% Revision:     1.0
% License: MIT  (opensource.org/licenses/MIT)
%--------------------------------------------------------------------------
function modelParams = createFFTFilters(modelParams)

modelParams.fftROMDataType = numerictype(1,16,8);
%----------------------------------------------------------------------------
% Filter 1 - BASS (filterSelect=0)
%----------------------------------------------------------------------------
modelParams.bandPassCutoffLow_1  = 0;  % frequency in Hz BASS
modelParams.bandPassCutoffHigh_1 = 300;  % frequency in Hz BASS
modelParams.bandPassIndexLow_1   = ceil(modelParams.bandPassCutoffLow_1 / modelParams.audio.sampleFrequency * modelParams.fft.size) + 1;  % Note: The FFT bin indexing assumes the first bin index number of zero.  Matlab starts arrays with index 1.  Thus we need to add get the right frequency bin.
modelParams.bandPassIndexHigh_1  = floor(modelParams.bandPassCutoffHigh_1 / modelParams.audio.sampleFrequency *modelParams.fft.size)+ 1;
modelParams.fftGainsF1Real     = zeros(modelParams.fft.sizeHalf,1);  % start by zeroing array
modelParams.fftGainsF1Real(modelParams.bandPassIndexLow_1:modelParams.bandPassIndexHigh_1)  = ones(modelParams.bandPassIndexHigh_1-modelParams.bandPassIndexLow_1+1,1);
modelParams.fftGainsF1Imag     = zeros(modelParams.fft.sizeHalf,1);  % zero imaginary part
modelParams.fftGainsF1Real     = fi(modelParams.fftGainsF1Real,modelParams.fftROMDataType);
modelParams.fftGainsF1Imag     = fi(modelParams.fftGainsF1Imag,modelParams.fftROMDataType);

%----------------------------------------------------------------------------
% Filter 2 - LOW MIDS (filterSelect=1)
%----------------------------------------------------------------------------

modelParams.bandPassCutoffLow_2  = 301;  % frequency in Hz LOW MIDS
modelParams.bandPassCutoffHigh_2 = 2000;  % frequency in Hz LOW MIDS
modelParams.bandPassIndexLow_2   = ceil(modelParams.bandPassCutoffLow_2 / modelParams.audio.sampleFrequency * modelParams.fft.size) + 1;  % Note: The FFT bin indexing assumes the first bin index number of zero.  Matlab starts arrays with index 1.  Thus we need to add get the right frequency bin.
modelParams.bandPassIndexHigh_2  = floor(modelParams.bandPassCutoffHigh_2 / modelParams.audio.sampleFrequency *modelParams.fft.size)+ 1;
modelParams.fftGainsF2Real     = zeros(modelParams.fft.sizeHalf,1);  % start by zeroing array
modelParams.fftGainsF2Real(modelParams.bandPassIndexLow_2:modelParams.bandPassIndexHigh_2)  = ones(modelParams.bandPassIndexHigh_2-modelParams.bandPassIndexLow_2+1,1);
modelParams.fftGainsF2Imag     = zeros(modelParams.fft.sizeHalf,1);  % zero imaginary part
modelParams.fftGainsF2Real     = fi(modelParams.fftGainsF2Real,modelParams.fftROMDataType);
modelParams.fftGainsF2Imag     = fi(modelParams.fftGainsF2Imag,modelParams.fftROMDataType);

%----------------------------------------------------------------------------
% Filter 3 - HIGH MIDS (filterSelect=2)
%----------------------------------------------------------------------------

modelParams.bandPassCutoffLow_3  = 2001;  % frequency in Hz HIGH MIDS
modelParams.bandPassCutoffHigh_3 = 4000;  % frequency in Hz HIGH MIDS
modelParams.bandPassIndexLow_3   = ceil(modelParams.bandPassCutoffLow_3 / modelParams.audio.sampleFrequency * modelParams.fft.size) + 1;  % Note: The FFT bin indexing assumes the first bin index number of zero.  Matlab starts arrays with index 1.  Thus we need to add get the right frequency bin.
modelParams.bandPassIndexHigh_3  = floor(modelParams.bandPassCutoffHigh_3 / modelParams.audio.sampleFrequency *modelParams.fft.size)+ 1;
modelParams.fftGainsF3Real     = zeros(modelParams.fft.sizeHalf,1);  % start by zeroing array
modelParams.fftGainsF3Real(modelParams.bandPassIndexLow_3:modelParams.bandPassIndexHigh_3)  = ones(modelParams.bandPassIndexHigh_3-modelParams.bandPassIndexLow_3+1,1);
modelParams.fftGainsF3Imag     = zeros(modelParams.fft.sizeHalf,1);  % zero imaginary part
modelParams.fftGainsF3Real     = fi(modelParams.fftGainsF3Real,modelParams.fftROMDataType);
modelParams.fftGainsF3Imag     = fi(modelParams.fftGainsF3Imag,modelParams.fftROMDataType);

%----------------------------------------------------------------------------
% Filter 4 - PRESENCE (filterSelect=3)
%----------------------------------------------------------------------------

modelParams.bandPassCutoffLow_4  = 4001;  % frequency in Hz PRESENCE
modelParams.bandPassCutoffHigh_4 = 6000;  % frequency in Hz PRESENCE
modelParams.bandPassIndexLow_4   = ceil(modelParams.bandPassCutoffLow_4 / modelParams.audio.sampleFrequency * modelParams.fft.size) + 1;  % Note: The FFT bin indexing assumes the first bin index number of zero.  Matlab starts arrays with index 1.  Thus we need to add get the right frequency bin.
modelParams.bandPassIndexHigh_4  = floor(modelParams.bandPassCutoffHigh_4 / modelParams.audio.sampleFrequency *modelParams.fft.size)+ 1;
modelParams.fftGainsF4Real     = zeros(modelParams.fft.sizeHalf,1);  % start by zeroing array
modelParams.fftGainsF4Real(modelParams.bandPassIndexLow_4:modelParams.bandPassIndexHigh_4)  = ones(modelParams.bandPassIndexHigh_4-modelParams.bandPassIndexLow_4+1,1);
modelParams.fftGainsF4Imag     = zeros(modelParams.fft.sizeHalf,1);  % zero imaginary part
modelParams.fftGainsF4Real     = fi(modelParams.fftGainsF4Real,modelParams.fftROMDataType);
modelParams.fftGainsF4Imag     = fi(modelParams.fftGainsF4Imag,modelParams.fftROMDataType);



%----------------------------------------------------------------------------
% Filter 5 - BRILLIANCE (filterSelect=4)
%----------------------------------------------------------------------------
modelParams.bandPassCutoffLow_5  = 6001;  % frequency in Hz BRILLIANCE
modelParams.bandPassCutoffHigh_5 = 30000;  % frequency in Hz BRILLIANCE
modelParams.bandPassIndexLow_5   = ceil(modelParams.bandPassCutoffLow_5 / modelParams.audio.sampleFrequency * modelParams.fft.size) + 1;  % Note: The FFT bin indexing assumes the first bin index number of zero.  Matlab starts arrays with index 1.  Thus we need to add get the right frequency bin.
modelParams.bandPassIndexHigh_5  = floor(modelParams.bandPassCutoffHigh_5 / modelParams.audio.sampleFrequency *modelParams.fft.size)+ 1;
modelParams.fftGainsF5Real     = zeros(modelParams.fft.sizeHalf,1);  % start by zeroing array
modelParams.fftGainsF5Real(modelParams.bandPassIndexLow_5:modelParams.bandPassIndexHigh_5)  = ones(modelParams.bandPassIndexHigh_5-modelParams.bandPassIndexLow_5+1,1);
modelParams.fftGainsF5Imag     = zeros(modelParams.fft.sizeHalf,1);  % zero imaginary part
modelParams.fftGainsF5Real     = fi(modelParams.fftGainsF5Real,modelParams.fftROMDataType);
modelParams.fftGainsF5Imag     = fi(modelParams.fftGainsF5Imag,modelParams.fftROMDataType);
end