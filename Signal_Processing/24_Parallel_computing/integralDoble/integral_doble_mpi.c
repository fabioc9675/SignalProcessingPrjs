#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

typedef struct { 
    double x, y, z, fval; 
} Point;

double compute_distance(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dz = p1.z - p2.z;
    return sqrt(dx*dx + dy*dy + dz*dz + 1e-15);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Point *data1 = NULL, *data2 = NULL;
    int N, M;
    double dV1 = 0.1 * 0.1 * 0.1, dV2 = 0.1 * 0.1 * 0.1;

    // ------------------------------------------
    // Paso 1: Definir el tipo MPI para Point
    // ------------------------------------------
    MPI_Datatype MPI_Point;
    int block_lengths[4] = {1, 1, 1, 1};
    MPI_Aint displacements[4];
    MPI_Datatype types[4] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};

    // Calcular desplazamientos de los campos del struct
    displacements[0] = offsetof(Point, x);
    displacements[1] = offsetof(Point, y);
    displacements[2] = offsetof(Point, z);
    displacements[3] = offsetof(Point, fval);

    MPI_Type_create_struct(4, block_lengths, displacements, types, &MPI_Point);
    MPI_Type_commit(&MPI_Point);

    // ------------------------------------------
    // Paso 2: Leer datos en el proceso 0
    // ------------------------------------------
    if (rank == 0) {
        FILE *file1 = fopen("data1.txt", "r");
        data1 = malloc(500000 * sizeof(Point));
        N = 0;
        while (fscanf(file1, "%lf %lf %lf %lf", &data1[N].x, &data1[N].y, &data1[N].z, &data1[N].fval) == 4) N++;
        fclose(file1);

        FILE *file2 = fopen("data2.txt", "r");
        data2 = malloc(500000 * sizeof(Point));
        M = 0;
        while (fscanf(file2, "%lf %lf %lf %lf", &data2[M].x, &data2[M].y, &data2[M].z, &data2[M].fval) == 4) M++;
        fclose(file2);
    }

    // ------------------------------------------
    // Paso 3: Compartir N y M con todos los procesos
    // ------------------------------------------
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // ------------------------------------------
    // Paso 4: Distribuir data2 a todos los procesos
    // ------------------------------------------
    if (rank != 0) {
        data2 = malloc(M * sizeof(Point));
    }
    MPI_Bcast(data2, M, MPI_Point, 0, MPI_COMM_WORLD);

    // ------------------------------------------
    // Paso 5: Distribuir data1 entre procesos
    // ------------------------------------------
    int local_N = N / size;
    Point *local_data1 = malloc(local_N * sizeof(Point));

    MPI_Scatter(data1, local_N, MPI_Point,
                local_data1, local_N, MPI_Point,
                0, MPI_COMM_WORLD);

    // ------------------------------------------
    // Paso 6: Calcular contribución local
    // ------------------------------------------
    double local_total = 0.0;
    for (int i = 0; i < local_N; i++) {
        double f1_sq = local_data1[i].fval * local_data1[i].fval;
        for (int j = 0; j < M; j++) {
            double dist = compute_distance(local_data1[i], data2[j]);
            double f2_sq = data2[j].fval * data2[j].fval;
            local_total += (f1_sq * f2_sq) / dist * dV1 * dV2;
        }
    }

    // ------------------------------------------
    // Paso 7: Reducir resultados al proceso 0
    // ------------------------------------------
    double global_total;
    MPI_Reduce(&local_total, &global_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // ------------------------------------------
    // Paso 8: Liberar memoria y finalizar
    // ------------------------------------------
    if (rank == 0) {
        printf("Integral Doble (MPI): %.6e\n", global_total);
        free(data1);
    }

    free(data2);
    free(local_data1);
    MPI_Type_free(&MPI_Point);
    MPI_Finalize();
    return 0;
}