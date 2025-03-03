#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main() {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return 1;
    }

    double posx, posy, posz, fval;
    double sum = 0.0;
    double dV = 0.1 * 0.1 * 0.1; // Ajusta según tu grilla
    clock_t start = clock();

    while (fscanf(file, "%lf %lf %lf %lf", &posx, &posy, &posz, &fval) == 4) {
        double r = sqrt(posx*posx + posy*posy + posz*posz);
        if (r > 1e-9) { // Evita división por cero
            sum += (fval / r) * dV;
        }
    }

    fclose(file);
    double time_spent = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("Integral (Serial): %.6f\n", sum);
    printf("Tiempo: %.6f segundos\n", time_spent);
    return 0;
}