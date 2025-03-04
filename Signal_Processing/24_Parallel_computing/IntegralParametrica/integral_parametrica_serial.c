#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


//#include <math.h>  // Para M_PI (si no está definido, usar #define PI 3.14159265358979323846)

// Constantes fundamentales
const double h = 6.62606876e-34;         // Constante de Planck [J*s]
const double pi = M_PI;                   // Pi
const double hbar = h/(2.0*pi);          // Constante de Planck reducida [J*s]
const double epsilon0 = 8.854187817e-12; // Permitividad del vacío [F/m]
const double xme0 = 9.10938188e-31;      // Masa del electrón [kg]
const double c = 299792458.0;            // Velocidad de la luz [m/s]
const double e = 1.60217662e-19;         // Carga elemental [C]
const double xJeV = 1.0/1.60217646e-19;  // Conversión Joules a eV

// Constantes específicas del GaAs
const double epsilon_GaAs = 10.2*epsilon0; // Permitividad del GaAs [F/m]

// Factor de conversión a meV (¡esta es la constante que debes multiplicar por tu integral!)
#define constante ((e) * (e) / (4.0 * pi * epsilon_GaAs) * xJeV * 1e3)

typedef struct {
    double x, y, z;
    double fval_e[30]; // Almacena fval_e1 a fval_e30
} Point;

double compute_distance(Point p1, Point p2) {
    double dx = (p1.x - p2.x) * 1e-9;
    double dy = (p1.y - p2.y) * 1e-9;
    double dz = (p1.z - p2.z) * 1e-9;
    return sqrt(dx*dx + dy*dy + dz*dz + 1e-20);
}

int main() {
    clock_t start = omp_get_wtime();

    // Leer data1.txt
    FILE *file1 = fopen("data1.txt", "r");
    Point *data1 = malloc(19783 * sizeof(Point));
    int N = 0;
    while (fscanf(file1, "%lf %lf %lf", &data1[N].x, &data1[N].y, &data1[N].z) == 3) {
        for (int k = 0; k < 30; k++) {
            fscanf(file1, "%lf", &data1[N].fval_e[k]);
        }
        N++;
    }
    fclose(file1);

    // Leer data2.txt
    FILE *file2 = fopen("data2.txt", "r");
    Point *data2 = malloc(19783 * sizeof(Point));
    int M = 0;
    while (fscanf(file2, "%lf %lf %lf", &data2[M].x, &data2[M].y, &data2[M].z) == 3) {
        for (int k = 0; k < 30; k++) {
            fscanf(file2, "%lf", &data2[M].fval_e[k]);
        }
        M++;
    }
    fclose(file2);

    double dV1 = 0.4 * 0.4 * 0.4, dV2 = 0.4 * 0.4 * 0.4;
    double totals[30] = {0.0}; // Resultados para los 30 parámetros
    
    clock_t start_int = omp_get_wtime();

    double xnorm_e[30] = {0.0}, xnorm_h[30] = {0.0};
    for (int k = 0; k < 30; k++) {
        for (int i = 0; i < N; i++) xnorm_e[k] += data1[i].fval_e[k] * data1[i].fval_e[k] * dV1;
        for (int j = 0; j < M; j++) xnorm_h[k] += data2[j].fval_e[k] * data2[j].fval_e[k] * dV2;
    }

    printf("Tiempo integral norm: %.2f segundos\n", (double)(clock() - start_int) / CLOCKS_PER_SEC);
    start_int = clock();

    // Calcular la integral CORRECTAMENTE
    for (int k = 0; k < 30; k++) {
        totals[k] = 0.0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                double r = compute_distance(data1[i], data2[j]);
                if (r < 1e-15) continue;
                double psi1_sq_norm = (data1[i].fval_e[k] * data1[i].fval_e[k]) / xnorm_e[k];
                double psi2_sq_norm = (data2[j].fval_e[k] * data2[j].fval_e[k]) / xnorm_h[k];
                totals[k] += (psi1_sq_norm * psi2_sq_norm) / r * dV1 * dV2;
            }
        }
        totals[k] *= constante; // Escalar a meV
        printf("Parámetro %d: %.6e meV, time: %.2f seg\n", k+1, totals[k], (double)(clock() - start) / CLOCKS_PER_SEC);
        start_int = clock();
    }


    printf("Tiempo: %.2f segundos\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    free(data1);
    free(data2);
    return 0;
}