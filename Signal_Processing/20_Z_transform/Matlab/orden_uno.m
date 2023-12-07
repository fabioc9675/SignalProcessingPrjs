clear all, close all, clc

s = tf(['s']);
Gs = 1/(s+1);  %% primer orden
Ts = 0.25;

Gsd = c2d(Gs,Ts,'zoh'); % transforma la TF de continuo a discreto
tsim = 20;
step(Gs,tsim); hold on

%% Respuesta al sistema de primer orden

u = ones(1,tsim/Ts);
x = zeros(1,tsim/Ts);

k = 0:Ts:tsim-Ts;

for i=2:length(k)
    x(i)=exp(-Ts)*x(i-1)+(1-exp(-Ts))*u(i-1);

end

stairs(k,x,'r')

    


