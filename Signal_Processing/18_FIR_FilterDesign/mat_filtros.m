fs = 100; % Frecuencia de muestreo
f_pass = 12.5; % Frecuencia de paso
% f_stop = 200; % Frecuencia de rechazo
passband = f_pass / (fs/2);
%stopband = f_stop / (fs/2);
N = 50; % Orden del filtro

% Diseño del filtro pasa bajos con fir1
b = fir1(N, passband, 'low', hanning(N+1));

% Respuesta en frecuencia del filtro diseñado
% freqz(b, 1, 512, fs);

T = readtable("low_0_25.csv");
scr = table2array(T(:,{'scr'}))';
lib = table2array(T(:,{'lib'}))';

figure(1)
freqz(b, 1, 512, fs);
figure(2)
freqz(scr, 1, 512, fs);
figure(3)
freqz(lib, 1, 512, fs);

%%

fs = 100; % Frecuencia de muestreo
f_pass = 30; % Frecuencia de paso
% f_stop = 200; % Frecuencia de rechazo
passband = f_pass / (fs/2);
%stopband = f_stop / (fs/2);
N = 50; % Orden del filtro

% Diseño del filtro pasa bajos con fir1
b = fir1(N, passband, 'high', hanning(N+1));

% Respuesta en frecuencia del filtro diseñado
% freqz(b, 1, 512, fs);

T = readtable("high_0_6.csv");
scr = table2array(T(:,{'scr'}))';
lib = table2array(T(:,{'lib'}))';

figure(1)
freqz(b, 1, 512, fs);
figure(2)
freqz(scr, 1, 512, fs);
figure(3)
freqz(lib, 1, 512, fs);

%%
fs = 100; % Frecuencia de muestreo
f_pass = 10; % Frecuencia de paso
f_stop = 25; % Frecuencia de rechazo
passband = f_pass / (fs/2);
stopband = f_stop / (fs/2);
N = 100; % Orden del filtro

% Diseño del filtro pasa bajos con fir1
b = fir1(N, [passband stopband], 'bandpass', hanning(N+1));

% Respuesta en frecuencia del filtro diseñado
% freqz(b, 1, 512, fs);

T = readtable("band_0_2__0_5.csv");
scr = table2array(T(:,{'scr'}))';
lib = table2array(T(:,{'lib'}))';

figure(1)
freqz(b, 1, 512, fs);
figure(2)
freqz(scr, 1, 512, fs);
figure(3)
freqz(lib, 1, 512, fs);
