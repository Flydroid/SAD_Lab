clear all;
close all;
    if ~isempty(instrfind)
     fclose(instrfind);
      delete(instrfind);
end



s = serial('com3','BaudRate',115200);
s.InputBufferSize = 512*10;
s.Terminator = 'CR';
s.ReadAsyncMode = 'continuous';

samples = s.InputBufferSize;
data = zeros(512,1);

fopen(s);
s_rate = 100;
fprintf(s,'%c','r\n');
n_rdy = 0;
n_read = 1;
pause(2);
while s.BytesAvailable() >= 0
    if s.BytesAvailable() > 0
        n_rdy = s.BytesAvailable();
        dat = fscanf(s,'%d',n_rdy)
        data(n_read) = dat;
        n_read = n_read+1;
        % dat_size = size(dat);
        %data(:,n_read:dat_size(1)) = dat;
        %n_read = s.ValuesReceived();
    end
    
end

fclose(s);
delete(s);

% Parameters
timeLimits = seconds([0 s_rate*512]); % seconds
frequencyLimits = [0 s_rate/2]; % Hz

%%
% Index into signal time region of interest
VarName1_ROI = data(:);
sampleRate = s_rate; % Hz
startTime = 0; % seconds
timeValues = startTime + (0:length(VarName1_ROI)-1).'/sampleRate;
VarName1_ROI = timetable(seconds(timeValues(:)),VarName1_ROI,'VariableNames',{'Data'});
VarName1_ROI = VarName1_ROI(timerange(timeLimits(1),timeLimits(2)),1);

% Compute spectral estimate
% Run the function call below without output arguments to plot the results
pspectrum(VarName1_ROI,'FrequencyLimits',frequencyLimits);
