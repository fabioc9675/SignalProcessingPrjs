

gcc integral_serial.c -o integral_serial

time ./integral_serial


gcc -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include integral_openmp.c -o integral_openmp -L/usr/local/opt/libomp/lib -lomp

time OMP_NUM_THREADS=4 ./integral_openmp



mpicc integral_mpi.c -o integral_mpi

time mpirun -np 4 ./integral_mpi  # Usa 4 procesos
