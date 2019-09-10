%% This m-file implements the simulation of a noise cancelling 
% adaptive filter. 
%
% This version is intended for those without access to the MATLAB
% Communications toolbox. The chirp (FM slide) is created and stored 
% in the chirpSignal.mat file. This variable loads the noise variable
% prior to it being added to a voice signal as an interferring or 
% noise signal. The adaptive filter attempts to remove the chirp signal.

% by Dr. Thad B. Welch, P.E. with minor tweaks by CHGW
%
% first created - 12 February 2012
% last updated - 10 May 2016

%% Declarations and adaptive filter preparation
clear;
N = 20; % number of adaptive filter coefficents (order N-1)
mu = 0.01; % convergence factor 
myFontSize = 16; % font size 
% Set font size for the plot labels globally
set(0,'DefaultAxesFontSize',myFontSize);
set(0,'DefaultTextFontSize',myFontSize);

% load the chirp signal and read in the recorded voice signal
load('chirpSignal.mat'); % the chirp noise data array
[voice, Fs] = audioread('voiceRecording.wav'); 
voice = voice'; % convert the column to a row

M = length(voice); % number of samples to be simulated
r(2:N) = noise(N-1:-1:1); % create the noise array, flipped around
w = zeros(1, N); % initialize the adaptive filter coefficents

xStorage = voice + noise; % create the signal plus noise
xStorage = xStorage/max(abs(xStorage)); % normalize xStorage
yStorage = zeros(1, M); % array for filtered noise
eStorage = zeros(1, M); % array for the "cleaned up" signal

%%  Algorithm for the adaptive filtering 
for j = N:M
    %%%% ISR simulation starts here: input the two channels of data
    r(1) = noise(j); % interference (correlated noise) 
    x = xStorage(j); % voice + interference 
    
    % adaptively filter the interference signal
    y = 0;
    for i = 0:N-1
        y = y + w(i+1)*r(N-i);
    end
    
    % error signal is the filtered voice output
    e = x - y;
    
    % Widrow-Hoff LMS algorithm: update the coefficients
    for i = 1:N
        w(i) = w(i) + 2*mu*e*r(N-i+1);
    end
    
    % prepare the r array for the next input sample
    for i = N:-1:2
        r(i) = r(i-1);
    end
    %%%% the ISR simulation ends here
    
    % storage for post simulation use
    yStorage(j) = y;
    eStorage(j) = e;
end

%% Listen to the results
% the original voice signal
soundsc(voice, Fs)

% the voice signal with interference
pause(24)
soundsc(xStorage, Fs)

% the adaptive filter's output 
% voice signal with the interference mitigated
pause(24)
soundsc(eStorage, Fs)

%% Plots
close all;
% plot of the voice signal with the interference
figure(1) % expand to full screen for better viewing
subplot(3,1,1)
plot(voice/max(abs(voice))) % the original voice signal
ylabel('original voice signal')
axis([0 M -1.1 1.1])
subplot(3,1,2)
plot(xStorage) % voice plus noise
ylabel('voice signal with the added chirp')
axis([0 M -1.1 1.1])
subplot(3,1,3)
plot(eStorage/max(abs(eStorage))) % the recovered signal
ylabel('recovered voice signal')
axis([0 M -1.1 1.1])
xlabel('sample number')

figure(2)
% normalized plots ... zoom in to compare the results
plot(voice/max(abs(voice)), 'b') % the original voice signal
hold on
plot(eStorage/max(abs(eStorage)), 'r') % the recovered signal
ylabel('voice signal')
xlabel('sample number')
legend('original voice signal', 'recovered voice signal')
axis([0 M -1.1 1.1])

figure(1)

% The font settings are pesistent until you restart MATLAB
% To reset manually, use:
% %%%%%%%%% reset default figure properties %%%%%%
set(0,'DefaultAxesFontSize','remove'); 
set(0,'DefaultTextFontSize','remove');
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
