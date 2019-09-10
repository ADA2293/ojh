% function-based SOS filter simulation ... DF-I implementation

% by Dr. Thad B. Welch, P.E.
% copyright 2012, written 18 July 2012

clear
input = [1 0 0 0 0 0 0 0 0 0]; % impulse
index = length(input); % length of x
yStorage = zeros(1, index);

x = [0 0 0];    % input  storage
y = [0 0 0];    % output storage

G = 0.248341078962541; % filter's gain
B = [1.0 2.0 1.0]; % numerator coefficients
A = [1.0 -0.184213803077536 0.177578118927698]; % denominator coefficients

for n = 1:index % ISR simulation
    % read in the current input value
    x(1) = input(n); 
    
    % perform the filtering operation 
    [x, y] = sosFiltFunDFI(x, y, B, A, G);
    
    % update the storage array (save the filter's output values)
    % note: this is not part of the ISR and is only needed for plotting
    yStorage(n) = y(1);
end

figure(1)
freqz(G*B, A)

figure(2)
impz(G*B, A, 10)
hold on
plot(0:9, yStorage, 'r*')
legend('   impz','sos routine')
hold off