#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct { double x, y, z, fval; } Point;

double compute_distance(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dz = p1.z - p2.z;
    return sqrt(dx*dx + dy*dy + dz*dz + 1e-15); // Evitar división por cero
}

int main() {
    clock_t start = clock();

    // Leer data1.txt
    FILE *file1 = fopen("data1.txt", "r");
    Point *data1 = malloc(1000000 * sizeof(Point));
    int N = 0;
    while (fscanf(file1, "%lf %lf %lf %lf", &data1[N].x, &data1[N].y, &data1[N].z, &data1[N].fval) == 4) N++;
    fclose(file1);

    // Leer data2.txt
    FILE *file2 = fopen("data2.txt", "r");
    Point *data2 = malloc(1000000 * sizeof(Point));
    int M = 0;
    while (fscanf(file2, "%lf %lf %lf %lf", &data2[M].x, &data2[M].y, &data2[M].z, &data2[M].fval) == 4) M++;
    fclose(file2);

    double dV1 = 0.1 * 0.1 * 0.1; // Ajustar según tu grilla
    double dV2 = 0.1 * 0.1 * 0.1;
    double total = 0.0;

    // Calcular la integral doble (O(N*M))
    for (int i = 0; i < N; i++) {
        double f1_sq = data1[i].fval * data1[i].fval;
        for (int j = 0; j < M; j++) {
            double dist = compute_distance(data1[i], data2[j]);
            double f2_sq = data2[j].fval * data2[j].fval;
            total += (f1_sq * f2_sq) / dist * dV1 * dV2;
        }
    }

    double time_spent = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Integral Doble (Serial): %.6e\n", total);
    printf("Tiempo: %.2f segundos\n", time_spent);

    free(data1);
    free(data2);
    return 0;
}