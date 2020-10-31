#include <time.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  int rank, size;
  double startTime,endTime;
  int sbuf, rbuf, temp;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status[size];
  MPI_Request req[size];
  startTime = MPI_Wtime(); 
  srand(time(NULL) + rank);
  sbuf = rand() % 50;
  printf("Process %i generated number : %d\n", rank, sbuf);
  for(int dest = rank; dest <= size-1; dest++){ // send to from its rank to n-1 
    MPI_Isend(&sbuf, 1, MPI_INT, dest, 0, MPI_COMM_WORLD,&req[rank]); 
  }
  for(int src = rank; src >= 0; src--){  
    MPI_Irecv(&temp, 1, MPI_INT, src, MPI_ANY_TAG, MPI_COMM_WORLD, &req[src]);
    rbuf += temp;
  }
  MPI_Waitall(rank+1,req,MPI_STATUS_IGNORE);
  printf("Process %i's partial sum using MPI_Scan : %d\n", rank, rbuf);
  endTime = MPI_Wtime();
  MPI_Finalize();
  printf("Process %i's execution Time: %lf\n", rank, endTime - startTime);
  return 0;
}
