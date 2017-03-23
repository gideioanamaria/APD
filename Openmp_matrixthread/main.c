#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define nrliniA 10           //numarul de lini din matricea A
#define nrcoloaneA 7        //numarul de coloane din matricea A
#define nrliniB 7           //numarul de lini din matricea B
#define nrcoloaneB 10        //numarul de coloane din matricea B

int matrice_A[50][50];
int matrice_B[50][50];
int matrice_Rezultat[50][50];

int main (int argc, char *argv[])
{
    int	tid, nthreads, i, j, k, chunk;
    int ok = 1;

    chunk = 2;                 //numarul de iteratii pe care le face fiecare thread

    omp_set_num_threads(8);    //seteaza numarul de thred-uri

    //Inceputul regiunii paralele
    //Shared : datele sunt partajate-vizibile si accesibile prin toate threadurile momentan
    //private: date private pentru fiecare fir, ceea ce înseamnă că fiecare fir va avea o copie locală
    #pragma omp parallel shared(matrice_A,matrice_B,matrice_Rezultat,nthreads,chunk) private(tid,i,j,k)
    {
        tid = omp_get_thread_num();  //se obtine tid
        //numai firul de master adica cel care ar tid=0 face asta
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Inmultirea matricilor cu %d thread-uri\n",nthreads);
        }

        /*Initializare matrici*/
        #pragma omp for schedule (static, chunk)
        for(i=0;i<nrliniA;i++)
        {
            for(j=0;j<nrcoloaneA;j++)
            {
                matrice_A[i][j] = rand()%10;
            }
        }
        #pragma omp for schedule (static, chunk)
        for(i=0;i<nrliniB;i++)
        {
            for(j=0;j<nrcoloaneB;j++)
            {
                matrice_B[i][j] = rand()%10;
            }
        }
        #pragma omp for schedule (static, chunk)
        for(i=0;i<nrliniA;i=i+2)
        {
            for(j=0;j<nrcoloaneB;j=j+2)
            {
                matrice_Rezultat[i][j]=0;
            }
        }

        if(nrcoloaneA!=nrliniB)
        {
            printf("\nInmultirea matricilor nu este posibila !!! \n");
            ok = 0;
        }
        else
        {
            //Afiseaza ce iteratii face fiecare thread
            printf("Thread %d start inmultire matrici...\n",tid);
            //schedule se foloseste pentru for
            //static- adica toate thredu-ri au impartite in mod egal iteratiile in functie de valoarea chunck
            #pragma omp for schedule (static, chunk)
            for (i=0; i<nrliniA; i++)
            {
                printf("Thread=%d  coloana=%d\n",tid,i);
                for(j=0; j<nrcoloaneB; j++)
                  for (k=0; k<nrcoloaneA; k++)
                     matrice_Rezultat[i][j]+=matrice_A[i][k] * matrice_B[k][j];
            }
        }
    }   //Sfarsitul regiunii paralele


    if(ok == 1)
    {

        printf("\n Rezultatul inmultiri matricilor A si B : \n");

        for(i=0;i<nrliniA;i++)
        {
            for(j=0;j<nrcoloaneB;j++)
            {
                printf("%d \t",matrice_Rezultat[i][j]);
            }
            printf("\n");
        }
    }
    return 0;

}
