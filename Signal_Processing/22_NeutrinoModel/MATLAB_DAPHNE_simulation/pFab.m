%% Generación de Datos Sintéticos
% Parámetros de la señal biexponencial
A1 = 2;          % Amplitud del primer término exponencial
A2 = 1;          % Amplitud del segundo término exponencial
alpha1 = 1;      % Tasa de decaimiento del primer término exponencial
alpha2 = 0.2;    % Tasa de decaimiento del segundo término exponencial
betha  = 0.2;
%t = (0:0.1:10)'; % Vector de tiempo (0 a 10 segundos con paso de 0.1 s)

% Generar la señal biexponencial con algo de ruido
%y = A1 * exp(-alpha1 * t) + A2 * exp(-alpha2 * t) + 0.1 * randn(size(t));

t = time(1:800)';
y = coldamp_signal_fbk(1,1:800)';

% Graficar los datos sintéticos
figure;
plot(t, y, 'o');
xlabel('Tiempo (s)');
ylabel('Amplitud');
title('Datos Sintéticos Biexponenciales con Ruido');
grid on;



%% Definición del Modelo de Ajuste Biexponencial
% Modelo biexponencial: y(t) = A1*exp(-alpha1*t) + A2*exp(-alpha2*t)
fitType = fittype('A1*exp(-alpha1*x) + A2*exp(-alpha2*x)', ...
                  'independent', 'x', ...
                  'coefficients', {'A1', 'alpha1', 'A2', 'alpha2'});


%% Ajuste del Modelo a los Datos
% Realizar el ajuste del modelo a los datos sintéticos
[curveFit, gof] = fit(t, y, fitType);

% Mostrar los coeficientes ajustados
disp('Coeficientes ajustados:');
disp(curveFit);

% Mostrar la bondad del ajuste (Goodness of Fit)
disp('Bondad del ajuste:');
disp(gof);



%% Visualización del Ajuste Biexponencial
% Graficar el ajuste junto con los datos originales
figure;
plot(curveFit, t, y);
xlabel('Tiempo (s)');
ylabel('Amplitud');
title('Ajuste Biexponencial');
legend('Datos', 'Ajuste Biexponencial');
grid on;



