#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define SIZE 4
int main (int argc, char *argv[])
{
	int numprocs, rank, sendcount, recvcount, source;
	int sendbuf[SIZE*4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	int recvbuf[SIZE];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if (numprocs == SIZE) {
		source = 1;
		sendcount = SIZE;
		recvcount = SIZE;
		MPI_Scatter(sendbuf,sendcount,MPI_INT,recvbuf,recvcount,MPI_INT,source,MPI_COMM_WORLD);
		printf("rank= %d Results: %d %d %d %d\n",rank,recvbuf[0], recvbuf[1],recvbuf[2],recvbuf[3]);
	} else
		printf("Must specify %d processors. Terminating.\n",SIZE);

	MPI_Finalize();
}