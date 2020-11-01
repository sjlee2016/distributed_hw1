#include <time.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv){
  int rank, size;
  double startTime,endTime;
  int sbuf, rbuf;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  startTime = MPI_Wtime(); 
  srand(time(NULL) + rank);
  sbuf = rand() % 50; //[-10,10] random integer
  printf("Process %i generated number : %d\n", rank, sbuf);
  MPI_Scan(&sbuf, &rbuf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("Process %i's partial sum using MPI_Scan : %d\n", rank, rbuf);
  endTime = MPI_Wtime();
  MPI_Finalize();
  printf("Process %i's execution Time: %lf\n", rank, endTime - startTime);
  return 0;
}
