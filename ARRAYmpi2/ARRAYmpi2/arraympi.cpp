#include <iostream>
#include "mpi.h"

using namespace std;

#define SIZE 4
#define N 16 
int main (int argc, char *argv[])
{
	int numprocs, rank, sendcount, recvcount;
	int buf[N]={3,5,3,4,7,3,8,6,8,7,6,8,5,3,1,2};
	int recvbuf[20];
	int poz[N],nrcautat=3;
	int j=0;
	int rezultat[20];
	int low=0;
	int high=0;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	
	if(N%numprocs!=0){
		cout<<"Numarul dat nu se imparte exact la numarul de procese!!"<<endl;
		MPI_Finalize();
		exit(0);	}	sendcount = N/numprocs;
	recvcount = N/numprocs;
	MPI_Scatter(buf,sendcount,MPI_INT,recvbuf,recvcount,MPI_INT,0,MPI_COMM_WORLD); //trimit cate o parte egala din vector la fiecare proces arr->recvbuf
	printf("Procesul %d : %d %d %d %d \n ",rank,recvbuf[0],recvbuf[1],recvbuf[2],recvbuf[3]);
	

	for(int i=0; i<recvcount; i++) {
		if(nrcautat==recvbuf[i]){
			poz[j]=i+(N/numprocs)*rank;
			cout<<"ID PROCES "<<rank<<" Pozitia: "<< i + (N/numprocs)*rank<<endl;
			j++;
		}
	}

	MPI_Gather(poz,SIZE,MPI_INT,rezultat,SIZE,MPI_INT,0,MPI_COMM_WORLD);//procesul o aduna toate datele te la toate procesele si le pune intr-un vector

	//if(rank==0){
		if(j!=NULL){
			cout<<"Pozitile pe care am gasit numarul in procesul " << rank <<" sunt: ";
			for(int i=0;i<j;i++){
				cout<<poz[i] <<"  ";
			}
		}
		else
			cout <<"Number not found!";
	//}

	MPI_Finalize();

	return 0;
}