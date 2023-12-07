clear all, close all, clc

%% Representacion continua de la funcion

A = [1];
B = [1 0.4 2];

sys = tf(A,B);
figure
step(sys)


%% Representacion discreta con ZOH

T = 0.25;
k = 0:T:30-T;


u_k = ones(1,length(k));
y_k = zeros(1,length(k));

a2 = 1.7871;
a3 = 0.9048;
c1 = 0.4468;
c2 = 0.4524;


for i=3:length(k)

    y_k(i) = a2*y_k(i-1) - a3*y_k(i-2) + c1*u_k(i-1) + c2*u_k(i-2);

end


figure
stairs(k,y_k)



