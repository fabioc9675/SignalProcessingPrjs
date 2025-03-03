#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

typedef struct {
    double posx, posy, posz, fval;
} Point;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Point *global_points = NULL;
    int count = 0;
    double dV = 0.1 * 0.1 * 0.1;

    // Proceso 0 lee los datos
    if (rank == 0) {
        FILE *file = fopen("data.txt", "r");
        if (!file) {
            perror("Error al abrir el archivo");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        Point tmp;
        while (fscanf(file, "%lf %lf %lf %lf", &tmp.posx, &tmp.posy, &tmp.posz, &tmp.fval) == 4) {
            count++;
        }
        rewind(file);

        global_points = malloc(count * sizeof(Point));
        for (int i = 0; i < count; i++) {
            fscanf(file, "%lf %lf %lf %lf", &global_points[i].posx, &global_points[i].posy, 
                                            &global_points[i].posz, &global_points[i].fval);
        }
        fclose(file);
    }

    // Broadcast del número de puntos
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Dividir y distribuir los datos
    int local_count = count / size;
    Point *local_points = malloc(local_count * sizeof(Point));

    // MPI_Scatter requiere enviar bloques de datos estructurados
    MPI_Datatype point_type;
    MPI_Type_contiguous(4, MPI_DOUBLE, &point_type);
    MPI_Type_commit(&point_type);

    MPI_Scatter(global_points, local_count, point_type,
                local_points, local_count, point_type,
                0, MPI_COMM_WORLD);

    // Calcular suma local
    double local_sum = 0.0;
    for (int i = 0; i < local_count; i++) {
        double r = sqrt(local_points[i].posx*local_points[i].posx +
                        local_points[i].posy*local_points[i].posy +
                        local_points[i].posz*local_points[i].posz);
        if (r > 1e-9) {
            local_sum += (local_points[i].fval / r) * dV;
        }
    }

    // Reducir sumas
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Integral (MPI): %.6f\n", global_sum);
        free(global_points);
    }

    free(local_points);
    MPI_Type_free(&point_type);
    MPI_Finalize();
    return 0;
}