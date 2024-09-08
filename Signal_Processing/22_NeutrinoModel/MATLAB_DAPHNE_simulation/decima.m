load('model.mat');

t = zeros(length(coldamp_signal), 1);

t(62) = 6e-5;


y = coldamp_signal(1:8:800);
x = t(1:8:800);
x(8) = 6e-5;


plot(x)
hold on
plot(y)