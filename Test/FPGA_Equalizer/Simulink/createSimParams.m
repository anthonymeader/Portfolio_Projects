% SPDX-License-Identifier: MIT
% Copyright (c) 2022 Trevor Vannoy, Ross K. Snider  All rights reserved.
%--------------------------------------------------------------------------
% Description:  Matlab Function to create/set Simulation parameters for a
%               Simulink model simulation
%--------------------------------------------------------------------------
% Authors:      Ross K. Snider, Trevor Vannoy
% Company:      Montana State University
% Create Date:  April 5, 2022
% Revision:     1.0
% License: MIT  (opensource.org/licenses/MIT)
%--------------------------------------------------------------------------
function simParams = createSimParams(modelParams)

%--------------------------------------------------------------------------
% Audio file for simulation
%--------------------------------------------------------------------------
filename = 'mysong.wav';
desiredFs = modelParams.audio.sampleFrequency;  % desired sample rate
desiredCh = 'left';  % desired channel 
desiredNt = modelParams.audio.dataType;  % desired data type (Numeric type)
simParams.audioIn = getAudio(filename, desiredFs, desiredCh, desiredNt);

%--------------------------------------------------------------------------
% Simulation Parameters
%--------------------------------------------------------------------------
simParams.verifySimulation = false;  
simParams.playOutput       = true;
simParams.stopTime         = 1; % seconds

passthrough = 0;  % set value and data type from model parameters
simParams.passthrough = fi(passthrough,modelParams.passthrough.dataType);

%filterSelect = 3;  % set value [0 3] and data type from model parameters
%simParams.filterSelect = fi(filterSelect,modelParams.filterSelect.dataType);

%--------------------------------------------------------------------------
% Model Parameters for simulation
%--------------------------------------------------------------------------
gain_1 = 0.0;%3;  % set value and data type from model parameters //gain of ~10db
simParams.gain_1 = fi(gain_1,modelParams.gain_1.dataType);

gain_2 = 0.0;  % set value and data type from model parameters //gain of ~-50db
simParams.gain_2= fi(gain_2,modelParams.gain_2.dataType);

gain_3 = 2.0;  % set value and data type from model parameters
simParams.gain_3= fi(gain_3,modelParams.gain_3.dataType);

gain_4 = 0.0; % set value and data type from model parameters
simParams.gain_4= fi(gain_4,modelParams.gain_4.dataType);

gain_5 = 2.0;
simParams.gain_5= fi(gain_5,modelParams.gain_5.dataType);

volume = 1;
simParams.volume= fi(volume,modelParams.volume.dataType);






