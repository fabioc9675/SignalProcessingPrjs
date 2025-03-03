#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


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
    double fval_e[30];
} Point;

double compute_distance(Point p1, Point p2) {
    double dx = (p1.x - p2.x) * 1e-9;
    double dy = (p1.y - p2.y) * 1e-9;
    double dz = (p1.z - p2.z) * 1e-9;
    return sqrt(dx*dx + dy*dy + dz*dz + 1e-20);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Crear tipo MPI para Point
    MPI_Datatype MPI_Point;
    int block_lengths[2] = {3, 30};
    MPI_Aint displacements[2];
    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};

    Point dummy;
    displacements[0] = 0;
    displacements[1] = (char*)&dummy.fval_e[0] - (char*)&dummy;

    MPI_Type_create_struct(2, block_lengths, displacements, types, &MPI_Point);
    MPI_Type_commit(&MPI_Point);

    // Leer datos (solo proceso 0)
    Point *data1 = NULL, *data2 = NULL;
    int N = 0, M = 0; // Inicializar N y M
    if (rank == 0) {
        // Leer data1.txt
        FILE *file1 = fopen("data1.txt", "r");
        data1 = malloc(19783 * sizeof(Point));
        while (fscanf(file1, "%lf %lf %lf", &data1[N].x, &data1[N].y, &data1[N].z) == 3) {
            for (int k = 0; k < 30; k++) {
                fscanf(file1, "%lf", &data1[N].fval_e[k]);
            }
            N++;
        }
        fclose(file1);

        // Leer data2.txt
        FILE *file2 = fopen("data2.txt", "r");
        data2 = malloc(19783 * sizeof(Point));
        while (fscanf(file2, "%lf %lf %lf", &data2[M].x, &data2[M].y, &data2[M].z) == 3) {
            for (int k = 0; k < 30; k++) {
                fscanf(file2, "%lf", &data2[M].fval_e[k]);
            }
            M++;
        }
        fclose(file2);
    }

    // Broadcast N y M
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Distribuir data2 a todos los procesos
    // Distribuir datos
    if(rank != 0) {
        data1 = malloc(N * sizeof(Point));
        data2 = malloc(M * sizeof(Point));
    }
    MPI_Bcast(data1, N*sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data2, M*sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);

    double dV1 = 0.4e-27, dV2 = 0.4e-27;
    double local_totals[30] = {0};
    double global_totals[30] = {0};
    // Dividir trabajo por parámetros
    int chunk = 30/size;
    int start_k = rank*chunk;
    int end_k = (rank == size-1) ? 30 : (rank+1)*chunk;

    for(int k=start_k; k<end_k; k++) {
        double xnorm_e = 0, xnorm_h = 0;
        for(int i=0; i<N; i++) xnorm_e += pow(data1[i].fval_e[k], 2) * dV1;
        for(int j=0; j<M; j++) xnorm_h += pow(data2[j].fval_e[k], 2) * dV2;

        for(int i=0; i<N; i++) {
            for(int j=0; j<M; j++) {
                double r = compute_distance(data1[i], data2[j]);
                if(r < 1e-15) continue;
                
                double psi1_sq = pow(data1[i].fval_e[k], 2)/xnorm_e;
                double psi2_sq = pow(data2[j].fval_e[k], 2)/xnorm_h;
                local_totals[k] += (psi1_sq * psi2_sq) / r * dV1 * dV2;
            }
        }
        local_totals[k] *= constante;
    }

    MPI_Reduce(local_totals, global_totals, 30, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        for(int k=0; k<30; k++) 
            printf("Parámetro %d: %.6e meV\n", k+1, global_totals[k]);
    }

    MPI_Finalize();
    free(data1); free(data2);
    return 0;
}