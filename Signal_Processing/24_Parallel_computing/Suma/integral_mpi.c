#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double *global_fvals = NULL;
    double *local_fvals = NULL;
    int count = 0;
    double dV = 0.1 * 0.1 * 0.1;

    // Solo el proceso 0 lee el archivo
    if (rank == 0) {
        FILE *file = fopen("data.txt", "r");
        if (!file) {
            perror("Error al abrir el archivo");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Contar líneas
        double tmp;
        while (fscanf(file, "%lf %lf %lf %lf", &tmp, &tmp, &tmp, &tmp) == 4) {
            count++;
        }
        rewind(file);

        global_fvals = malloc(count * sizeof(double));
        for (int i = 0; i < count; i++) {
            fscanf(file, "%lf %lf %lf %lf", &tmp, &tmp, &tmp, &global_fvals[i]);
        }
        fclose(file);
    }

    // Broadcast el número de elementos a todos los procesos
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Dividir los datos
    int local_count = count / size;
    local_fvals = malloc(local_count * sizeof(double));

    // Scatter los datos
    MPI_Scatter(global_fvals, local_count, MPI_DOUBLE, 
                local_fvals, local_count, MPI_DOUBLE, 
                0, MPI_COMM_WORLD);

    // Calcular suma local
    double local_sum = 0.0;
    for (int i = 0; i < local_count; i++) {
        local_sum += local_fvals[i];
    }

    // Reducir todas las sumas locales
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double integral = global_sum * dV;
        printf("Integral (MPI): %.6f\n", integral);
        free(global_fvals);
    }

    free(local_fvals);
    MPI_Finalize();
    return 0;
}