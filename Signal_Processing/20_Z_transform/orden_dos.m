clear all, close all, clc

s = tf(['s']);
Gs = 1/(s^2+0.4*s+2);  %% primer orden
Ts = 0.25;

Gsd = c2d(Gs,Ts,'zoh'); % transforma la TF de continuo a discreto
tsim = 30;
step(Gs,tsim); 

%% Respuesta al sistema de primer orden

u = ones(1,tsim/Ts);
x = zeros(1,tsim/Ts);

k = 0:Ts:tsim-Ts;

c1 = 2*exp(-0.2*Ts)*cos(1.4*Ts);
c2 = exp(-2*0.2*Ts);
c3 = (1/2)*exp(-0.2*Ts)*cos(1.4*Ts);
c4 = (1/2)*exp(-0.4*Ts);

for i=3:length(k)


    x(i)=c1*x(i-1)-c2*x(i-2)+c3*u(i-1)+c4*u(i-2);

end

figure
stairs(k,x,'r')

%%ylim([0 3])

    


