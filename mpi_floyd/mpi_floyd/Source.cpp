#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

using namespace std;

#define INF 100000;

int BLOCK_LOW(int i, int p, int n) {
    return (i*n)/p;
}

int BLOCK_HIGH(int i, int p, int n) {
    return ((i+1)*n)/p;
}

int BLOCK_SIZE(int i, int p, int n) {
    return BLOCK_HIGH(i,p,n) - BLOCK_LOW(i,p,n);
}

int BLOCK_OWNER(int k, int p, int n) {
    return (p*(k+1)-1)/n;
}

int main(int argc, char *argv[])
{
    int i, j, k;
    int my_rank, num_procs, size, offset;
	int **A;
    int n = 6;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
	size=BLOCK_SIZE(my_rank, num_procs, n);


    int* cnts = new int(num_procs*sizeof(int)); //numarul de elemente primite de fiecare proces
    int* displs = new int(num_procs*sizeof(int)); //Entry i specifies the displacement relative to recvbuf at which to place the incoming data from process i (significant only at root)
    for (i=0;i<num_procs;i++) {
        cnts[i] = BLOCK_SIZE(i, num_procs, n)*n;
        displs[i] = BLOCK_LOW(i, num_procs, n)*n;
    }

    if (my_rank==0) {
		//create a 2D array with n rows and n columns
        int *A_storage = (int*)malloc(n*n*sizeof(int));
        A = (int**)malloc(n*sizeof(int*));
        for (i=0;i<n;i++)
            A[i] = &A_storage[i*n];

        A[0][0] = 0;
        A[0][1] = 2;
        A[0][2] = 5;
        A[0][3] = INF;
		A[0][4] = INF;
		A[0][5] = INF;
        A[1][0] = INF;
        A[1][1] = 0;
        A[1][2] = 7;
        A[1][3] = 1;
		A[1][4] = INF;
		A[1][5] = 8;
        A[2][0] = INF;
        A[2][1] = INF;
        A[2][2] = 0;
        A[2][3] = 4;
		A[2][4] = INF;
		A[2][5] = INF;
        A[3][0] = INF;
        A[3][1] = INF;
        A[3][2] = INF;
        A[3][3] = 0;
		A[3][4] = 3;
		A[3][5] = INF;
		A[4][0] = INF;
        A[4][1] = INF;
        A[4][2] = 2;
        A[4][3] = INF;
		A[4][4] = 0;
		A[4][5] = 3;
		A[5][0] = INF;
        A[5][1] = 5;
        A[5][2] = INF;
        A[5][3] = 2;
		A[5][4] = 4;
		A[5][5] = 0;

        for (i=1; i<num_procs; i++) {
            MPI_Send(&A[0][displs[i]], cnts[i], MPI_INT, i, 1, MPI_COMM_WORLD);
        }

    }
      
    if (my_rank != 0)
        MPI_Recv(&A[0][0], size*n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    
    int root = 0;   /* Process controlling row to be bcast */
    int *tmp = (int*)malloc(n*sizeof(int));  /* Holds the broadcast row*/

    for (k=0; k<n; k++) {
        root = BLOCK_OWNER(k, num_procs, n);

        if (my_rank==root){
			offset = k - BLOCK_LOW(my_rank,num_procs,n);
            for (i=0; i<n; i++)
                tmp[i] = A[offset][i];
		}

        MPI_Bcast(tmp, n, MPI_INT, root, MPI_COMM_WORLD);
        for (i=0; i< BLOCK_SIZE(my_rank, num_procs, n); i++)
            for (j=0;j<n;j++)
                if (A[i][j] > A[i][k] + tmp[j])
                    A[i][j] = A[i][k] + tmp[j];
    }

	//Colectează în locații specifice din toate procesele dintr-un grup
    MPI_Gatherv(&A, size, MPI_INT, &A[0][0], cnts, displs, MPI_INT, 0, MPI_COMM_WORLD);

	free(cnts);
    free(displs);
	free(tmp);

	//afisam matricea celor mai scurte drumuri
     if (my_rank==0) {
         for (i=0; i<n; i++) {
             for(j=0;j<n;j++)    
                 printf("%d  ", A[i][j]);
             printf("\n");
         }
         printf("\n");
    }

    MPI_Finalize();

    return 0;
}
