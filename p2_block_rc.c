#include <time.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  int rank, size;
  double startTime,endTime;
  int sbuf, rbuf, temp;
  MPI_Status status;
  MPI_Request req;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  startTime = MPI_Wtime(); 
  srand(time(NULL) + rank);
  sbuf = rand() % 50;
  printf("Process %i generated number : %d\n", rank, sbuf);
  for(int dest = rank; dest <= size-1; dest++){ // send to from its rank to n-1 
    MPI_Send(&sbuf, 1, MPI_INT, dest, 0, MPI_COMM_WORLD); 
  }
  for(int src = rank; src >= 0; src--){  
    MPI_Recv(&temp, 1, MPI_INT, src, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    rbuf += temp;
  }
  printf("Process %i's partial sum using MPI_Scan : %d\n", rank, rbuf);
  endTime = MPI_Wtime();
  MPI_Finalize();
  printf("Process %i's execution Time: %lf\n", rank, endTime - startTime);
  return 0;
}
