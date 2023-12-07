clear all, close all, clc

%% Representacion continua de la funcion

A = [1];
B = [1 1];

sys = tf(A,B);
figure
step(sys)


%% Representacion discreta con ZOH

T = 0.25;
k = 0:T:10-T;


u_k = ones(1,length(k));
y_k = zeros(1,length(k));

c1 = exp(-T);
c2 = 1-exp(-T);


for i=2:length(k)

    y_k(i) = c1*y_k(i-1) + c2*u_k(i-1);

end


figure
stairs(k,y_k)



