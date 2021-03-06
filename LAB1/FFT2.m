% Compute power spectrum

% Generated by MATLAB(R) 9.3 and Signal Processing Toolbox 7.5.
% Generated on: 06-Apr-2018 10:36:14

% Parameters
timeLimits = seconds([0 0.5]); % seconds
frequencyLimits = [0 250]; % Hz

%%
% Index into signal time region of interest
Gen50Hz_ROI = Gen50Hz(:);
sampleRate = 500; % Hz
startTime = 0; % seconds
timeValues = startTime + (0:length(Gen50Hz_ROI)-1).'/sampleRate;
Gen50Hz_ROI = timetable(seconds(timeValues(:)),Gen50Hz_ROI,'VariableNames',{'Data'});
Gen50Hz_ROI = Gen50Hz_ROI(timerange(timeLimits(1),timeLimits(2)),1);

% Compute spectral estimate
% Run the function call below without output arguments to plot the results
[PGen50Hz_ROI, FGen50Hz_ROI] = pspectrum(Gen50Hz_ROI, ...
    'FrequencyLimits',frequencyLimits);

%%
% Index into signal time region of interest
Sine50Hz_ROI = Sine50Hz(:);
sampleRate = 500; % Hz
startTime = 0; % seconds
timeValues = startTime + (0:length(Sine50Hz_ROI)-1).'/sampleRate;
Sine50Hz_ROI = timetable(seconds(timeValues(:)),Sine50Hz_ROI,'VariableNames',{'Data'});
Sine50Hz_ROI = Sine50Hz_ROI(timerange(timeLimits(1),timeLimits(2)),1);

% Compute spectral estimate
% Run the function call below without output arguments to plot the results
[PSine50Hz_ROI, FSine50Hz_ROI] = pspectrum(Sine50Hz_ROI, ...
    'FrequencyLimits',frequencyLimits);
