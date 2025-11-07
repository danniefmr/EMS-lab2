% === Bilinear Transform of Given Analog Filter at fs = 200 Hz ===
clear; clc;

% Analog filter coefficients
b_a = [1.02884815e10];
a_a = [1, 5319, 1.1625957e7, 1.02884893617e10];

% Sampling frequency
fs = 4000;     % Hz
T = 1/fs;

% Prewarp frequency (optional, but recommended)
% For a low-pass, we can prewarp the -3 dB cutoff frequency
% Here we estimate analog cutoff from denominator roots if needed
% For simplicity, apply standard bilinear transform
[b_z, a_z] = bilinear(b_a, a_a, fs);

% Display digital filter frequency response
freqz(b_z, a_z, 2048, fs);
title('Digital IIR Filter (Bilinear Transform)');

% Print coefficients for Pico C code
fprintf('\nC code arrays:\n');
fprintf('float b[] = { ');
fprintf('%0.10ef, ', b_z);
fprintf('};\n');

fprintf('float a[] = { ');
fprintf('%0.10ef, ', a_z);
fprintf('};\n');