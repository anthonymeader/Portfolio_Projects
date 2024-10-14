% SPDX-License-Identifier: MIT
% Copyright (c) 2022 Ross K. Snider  All rights reserved.
%--------------------------------------------------------------------------
% Description:  Matlab Function to verify the FFTAnalysisSynthesis
%               Simulink model. 
%--------------------------------------------------------------------------
% Authors:      Ross K. Snider
% Company:      Montana State University
% Create Date:  June 8, 2022
% Revision:     1.0
% License: MIT  (opensource.org/licenses/MIT)
%--------------------------------------------------------------------------

%--------------------------------------------------------------------------
% Verification method depends on the input signal
%--------------------------------------------------------------------------

        s3 = double(audioOut');
        s3 = s3/max(s3(:));  % normalize magnitude since we want any errors to be coming from waveform shape differences

        spectrogram(s3(:,1),2500,2000,32768,modelParams.audio.sampleFrequency,'yaxis');






