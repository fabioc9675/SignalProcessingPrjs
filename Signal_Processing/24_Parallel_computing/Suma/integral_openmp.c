#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef struct {
    double posx, posy, posz, fval;
} Point;

int main() {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Contar líneas y leer datos
    int count = 0;
    Point tmp;
    while (fscanf(file, "%lf %lf %lf %lf", &tmp.posx, &tmp.posy, &tmp.posz, &tmp.fval) == 4) {
        count++;
    }
    rewind(file);

    Point *points = malloc(count * sizeof(Point));
    for (int i = 0; i < count; i++) {
        fscanf(file, "%lf %lf %lf %lf", &points[i].posx, &points[i].posy, &points[i].posz, &points[i].fval);
    }
    fclose(file);

    double sum = 0.0;
    double dV = 0.1 * 0.1 * 0.1;
    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < count; i++) {
        double r = sqrt(points[i].posx*points[i].posx + 
                        points[i].posy*points[i].posy + 
                        points[i].posz*points[i].posz);
        if (r > 1e-9) {
            sum += (points[i].fval / r) * dV;
        }
    }

    printf("Integral (OpenMP): %.6f\n", sum);
    printf("Tiempo: %.6f segundos\n", omp_get_wtime() - start);
    free(points);
    return 0;
}