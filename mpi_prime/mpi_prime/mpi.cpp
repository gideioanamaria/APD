
#include <iostream>
#include "mpi.h"
#include <string>
#define FirstRank 0
#define N 40

using namespace std;

bool isprime(int n)
{
	if (n <= 1) {
                return 0;
        }
        if (n == 2) {
                return true;
        }
        if (!(n % 2)) {
                return false;
        }
        for (int i = 2; i*i <= n; i++) {
                if (!(n % i)) {
                        return 0;
                }
        }
        return 1;
}


int main(int argc, char *argv[])
{
	int i;

    int  my_rank;       /* rank of process */
    int  noProcesses;   /* number of processes */
    int  nameSize;      /* length of name */

	int  parte=0;       /* piece for every processes*/
	int count =0;

	int SendData = 0;
	int ReceiveData = 0;

    char computerName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv); /*START MPI */

    /*Determines the size of the group associated with a communicator */
    MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);

    /*Determines the rank of the calling process in the communicator*/
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /*Gets the name of the processor*/
    MPI_Get_processor_name(computerName, &nameSize);

    if(my_rank == FirstRank){

		parte = N/noProcesses;

		cout<<"Hello from process "<< my_rank <<" of "<< noProcesses<<" processor on " <<computerName <<"\n\n";

		for( count= parte*FirstRank;count < parte*(FirstRank+1);count++)
		{
			if(isprime(count)){
				cout << count << "  " ;
			}
		}

		SendData = parte;
		for (i = 1; i < noProcesses; i++)
		{
			MPI_Send(&SendData,1,MPI_INT,i,1,MPI_COMM_WORLD);
		}

    }

	MPI_Status status;
	if(my_rank > FirstRank){
		MPI_Recv(&ReceiveData,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
		//cout<<"Receive data " <<ReceiveData;
		cout<<"Hello from process "<< my_rank <<" of "<< noProcesses<<" processor on " <<computerName <<"\n\n";
		for( count= ReceiveData*my_rank;count < ReceiveData*(my_rank+1);count++)
		{
			if(isprime(count)){
				cout << count << "  " ;
			}
		}
	}

    MPI_Finalize(); /* EXIT MPI */

    return 0;
}
