function [B, A, K] = filt2par(filt1);
%  function [B, A, K] = filt2par(filt1)
%
%  This function is ONLY indended for use with an IIR filter structures!
%  This is because the parallel implementation of an FIR filter does not
%  exist.  More precisely, if you execute the command, 
%
%  [R, P, K] = residuez(filt1.tf.num, filt1.tf.den);
%
%  for and FIR filter, the K values will be the FIR filter coefficients.
%
%  This function converts a filt1 data structure (as generated by Sptool)
%  into a B and an A matrix and a gain value (K).  Each row of these matrices
%  contain the B or A coefficients associated with a second order section (sos).
%  
%   INPUT
%   filt1 - data structure generated by exporting a filter design from Sptool
%
%  OUTPUTS
%     B   - matrix of numerator polynomial coefficients
%     A   - matrix of denominator polynomial coefficients
%     K   - gain coefficient
%
%  written by Dr. Thad B. Welch, PE {t.b.welch@ieee.org}
%  copyright 2003
%  completed on 14 March 2003 revision 1.2

% Simulation input/initialization
counter = 0;    % counter for the A or B vector entries
skip = 0;       % skip = 1 to skip the 2nd term in a complex conjugate pair
B = [];         % initialize the numerator polynomial matrix
A = [];         % initialize the denominator polynomial matrix

% Calculated terms
[R, P, K] = residuez(filt1.tf.num, filt1.tf.den);

for i = 1:length(P) % converting from residuez form to parallel implementation form
    if (skip == 0)  % if it's a conjugate pair, it's the first pole in the pair
        counter = counter + 1;
        if (i == length(P)) % if it's the last entry, the pole MUST be distinct
            B(counter, :) = [0 R(i)];
            A(counter, :) = [0 1 -P(i)];
        else % conjugate pole pairs test
            if (P(i) == conj(P(i+1))) % conjugate pole pair case
                skip = 1;
                B(counter, :) = real(R(i)*[1 -P(i+1)] + R(i+1)*[1 -P(i)]);
                A(counter, :) = real(conv([1 -P(i)], [1 -P(i+1)]));
            else % distinct pole case
                B(counter, :) = [0 R(i)];
                A(counter, :) = [0 1 -P(i)];
            end
        end
    else % second term in the conjugate pole pair ... skip it!
        skip = 0;
    end
end
