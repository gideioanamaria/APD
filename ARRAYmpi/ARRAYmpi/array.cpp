#include <iostream>
#include <mpi.h>
#include <math.h>

using namespace std;

#define MAXSIZE 20

int main(int argc, char *argv[])
{
    int  my_rank;       /* rank of process */
    int  nrprocs;   /* number of processes */

	int arr[MAXSIZE], i, x, low, high, myresult=0, result;
	int poz=0;
	int nrcautat=3;


    MPI_Init(&argc, &argv); /*START MPI */

    /*Determines the size of the group associated with a communicator */
    MPI_Comm_size(MPI_COMM_WORLD, &nrprocs);

    /*Determines the rank of the calling process in the communicator*/
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


	if(0 == my_rank) {
		cout<<"Enter Array Elements : ";
		for(i=0; i<MAXSIZE; i++){
			cin>>arr[i];
		}
	}

	 /* broadcast data */
	//trimite vectorul de la procesul root la toate celelalte procese
	//arr – data to be sent
	//MAXSIZE – size of the data buffer
	//MPI_INT – mpi data type
	//0 – sender
	//MPI_COMM_WORLD – communicator
	MPI_Bcast(arr, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);

	/* search the number in portion of data */
	x = MAXSIZE/nrprocs; /* must be an integer */
	low = my_rank * x;
	high = low + x;
	for(i=low; i<high; i++) {
		if(nrcautat==arr[i]){
			poz=i;
		}
	}
	cout<<"I got pozition " << poz << " from " << my_rank << "\n";

	 /* compute global MAX */
	/*poz - address of the send buffer
	1 - number of elements in send buffer
	MPI_INT- data type of elements of send buffer
	MPI_MAX - reduce operation
	0 - rank of root process
	MPI_COMM_WORLD– communicator
	result - address of the receive buffer*/
	MPI_Reduce(&poz, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if(0 == my_rank) {
		if(result==0)
		{
			cout<<"Number NOT found..!!\n";
		}
		else{
			cout<<"Number FOUND at MAX position "<< result <<"\n";
		}
	}


    MPI_Finalize(); /* EXIT MPI */

    return 0;
}
