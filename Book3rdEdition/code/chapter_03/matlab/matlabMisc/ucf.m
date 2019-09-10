%//////////////////////////////////////////////////////////////////////
%/ Welch, Wright, & Morrow, 
%/ Real-time Digital Signal Processing, 2005
%/
%/ ucf.m  ... unit circle fixer m-file 
%/
%/ This m-file corrects the zplane and zplaneplot commands improperly 
%/ plotted unit circle.  As currently written, these MATLAB commands plot 
%/ the unit circle using only 70 points.  This lack of plotting precision
%/ is at best misleading as poles and/or zeros approach the unit circle.
%/
%/ To change the linestyle or color, update the 'r' in line 20
%/
%//////////////////////////////////////////////////////////////////////

theta = linspace(0, 2*pi, 70);    % recreate zplaneplot's index
N = 100000;                       % number of points used to plot the unit circle

hold on
plot(cos(theta), sin(theta), 'w') % overwrite the unit circle in white
plot(exp(j*2*pi*(0:N)/N), 'r')    % plot the unit circle in red
hold off
