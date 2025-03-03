#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return 1;
    }

    double posx, posy, posz, fval;
    double sum = 0.0;
    int count = 0;

    // Leer los datos y acumular la suma
    while (fscanf(file, "%lf %lf %lf %lf", &posx, &posy, &posz, &fval) == 4) 
    {
        sum += fval;
        count ++;
    }

    fclose(file);

    // Asumir dV constante
    double dV = 0.1 * 0.1 * 0.1;
    double integral = sum * dV;

    printf("Integral (Serial): %.6f\n", integral);
    printf("Tiempo estimado: Usar time ./integral_serial\n");

    return 0;
}