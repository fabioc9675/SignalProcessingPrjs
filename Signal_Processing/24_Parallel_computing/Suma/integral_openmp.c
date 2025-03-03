#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Contar líneas para almacenar en un array
    int count = 0;
    double tmp;
    while (fscanf(file, "%lf %lf %lf %lf", &tmp, &tmp, &tmp, &tmp) == 4) {
        count++;
    }
    printf("Numero de datos = %d", count);
    rewind(file);


    double *fvals = malloc(count * sizeof(double));
    for (int i = 0; i < count; i++) {
        fscanf(file, "%lf %lf %lf %lf", &tmp, &tmp, &tmp, &fvals[i]);
    }
    fclose(file);

    double sum = 0.0;
    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < count; i++) {
        sum += fvals[i];
    }

    double dV = 0.1 * 0.1 * 0.1;
    double integral = sum * dV;

    printf("Integral (OpenMP): %.6f\n", integral);
    printf("Tiempo: %.6f segundos\n", omp_get_wtime() - start);

    free(fvals);
    return 0;


}